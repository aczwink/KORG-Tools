/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of KORG-Tools.
 *
 * KORG-Tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KORG-Tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KORG-Tools.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "ResourceImporter.hpp"

//Public methods
void ResourceImporter::ImportPerformance(const PerformanceBankNumber &bankNumber, uint8 pos)
{
	const auto& performanceEntry = this->sourceSet.performanceBanks[bankNumber][pos];
	const PerformanceObject& performanceObject = *performanceEntry.object;

	stdOut << u8"Importing performance: " << performanceEntry.name << endl;

	BankSlot<PerformanceBankNumber> sourceSlot = {bankNumber, pos};
	const auto slot = this->targetSet.performanceBanks.FindFreeSlot(this->config.performanceInsertSlot.HasValue() ? this->config.performanceInsertSlot.Value() : sourceSlot);
	if(!slot.HasValue())
	{
		stdOut << u8"No free performance slot could be found." << endl;
		return;
	}

	switch(performanceObject.Version())
	{
		case 0:
			this->ImportPerformanceData(performanceObject.V0Data(), *slot, performanceEntry.name);
			break;
		case 1:
			this->ImportPerformanceData(performanceObject.V1Data(), *slot, performanceEntry.name);
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	stdOut << u8"Successfully imported performance '" << performanceEntry.name << u8"' to slot: " << slot->bankNumber.ToString() << u8", " << slot->pos << endl;

	this->SaveChanges();
}

//Private methods
bool ResourceImporter::CheckIfSoundExists(const Sound::SoundData &soundData)
{
	for(const auto& bankEntry : this->targetSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(objectEntry.object->data == soundData)
				return true;
		}
	}
	return false;
}

bool ResourceImporter::ImportMultiSample(uint64 id)
{
	if(this->importedMultiSampleIds.Contains(id))
		return true;
	const auto& msEntry = this->sourceMultiSamplesIndex.GetMultiSampleEntryById(id);

	stdOut << u8"--Importing multisample: " << msEntry.name << endl;
	for(uint8 relativeIndex : msEntry.keyZoneIndex)
	{
		if(relativeIndex == Unsigned<uint8>::Max())
			continue;

		const auto& keyZone = this->sourceSet.MultiSamples().data.keyboardZones[msEntry.keyZoneBaseIndex + relativeIndex];
		if(keyZone.sampleNumber == -1)
			continue;

		if(!this->ImportSample(this->sourceSet.MultiSamples().data.sampleEntries[keyZone.sampleNumber].id))
			return false;
	}

	MultiSamples::MultiSampleEntry newMultiSampleEntry = msEntry;
	newMultiSampleEntry.keyZoneBaseIndex = this->targetSet.MultiSamples().data.keyboardZones.GetNumberOfElements();
	BinaryTreeSet<uint8> importedKeyZones;
	for(uint8& relativeIndex : newMultiSampleEntry.keyZoneIndex)
	{
		if(relativeIndex == Unsigned<uint8>::Max())
			continue;
		if(!importedKeyZones.Contains(relativeIndex))
		{
			const auto& srcKeyZone = this->sourceSet.MultiSamples().data.keyboardZones[msEntry.keyZoneBaseIndex + relativeIndex];
			MultiSamples::KeyboardZone keyboardZone = srcKeyZone;

			if(srcKeyZone.sampleNumber != -1)
				keyboardZone.sampleNumber = this->importedSampleIds.Get(this->sourceSet.MultiSamples().data.sampleEntries[srcKeyZone.sampleNumber].id);

			this->targetSet.MultiSamples().data.keyboardZones.Push(keyboardZone);
			importedKeyZones.Insert(relativeIndex);
		}
	}

	uint32 index = this->targetSet.MultiSamples().data.multiSampleEntries.Push(newMultiSampleEntry);
	this->importedMultiSampleIds.Insert(id, index);
	stdOut << u8"--Successfully imported multi sample: " << msEntry.name << endl;
	return true;
}

bool ResourceImporter::ImportSample(uint64 id)
{
	if(this->importedSampleIds.Contains(id))
		return true;
	const auto& sampleEntry = this->sourceMultiSamplesIndex.GetSampleEntryById(id);

	stdOut << u8"---Importing sample: " << sampleEntry.name << u8" (" << PitchToString(sampleEntry.originalNote) << u8")" << endl;
	auto sourceLocation = this->sourceSetIndex.GetSampleLocation(id);
	const auto slot = this->targetSet.sampleBanks.FindFreeSlot(sourceLocation);
	if(!slot.HasValue())
	{
		stdOut << u8"No free sample slot available" << endl;
		return false;
	}

	const auto& entry = this->sourceSet.sampleBanks[sourceLocation.bankNumber][sourceLocation.pos];
	this->targetSet.sampleBanks[slot->bankNumber].SetObject(entry.name, slot->pos, entry.object);

	uint32 index = this->targetSet.MultiSamples().data.sampleEntries.Push(sampleEntry);
	this->importedSampleIds.Insert(id, index);
	return true;
}

bool ResourceImporter::ImportSound(const ProgramChangeSequence &programChangeSequence)
{
	if(this->importedSounds.Contains(programChangeSequence))
		return true;
	auto sourceLocation = this->sourceSetIndex.GetSoundLocation(programChangeSequence);
	if(!sourceLocation.HasValue())
		return true;

	const auto& soundEntry = this->sourceSet.soundBanks[sourceLocation->bankNumber][sourceLocation->pos];
	const SoundObject& soundObject = *soundEntry.object;

	stdOut << u8"-Importing sound: " << soundEntry.name << endl;
	const auto slot = this->targetSet.soundBanks.FindFreeSlot(this->config.soundInsertSlot.HasValue() ? this->config.soundInsertSlot.Value() : *sourceLocation);
	if(!slot.HasValue())
	{
		stdOut << u8"No free sound slot available" << endl;
		return false;
	}

	if(!this->ImportSoundDependencies(soundObject.data))
		return false;

	Sound::SoundData newSound = soundObject.data;
	for(auto& osc : newSound.oscillators)
	{
		if(osc.low.source == Sound::MultiSampleSource::RAM)
			osc.low.multiSampleNumber = this->MapMultiSampleIdToIndex(osc.low.multiSampleId);
		if(osc.high.source == Sound::MultiSampleSource::RAM)
			osc.low.multiSampleNumber = this->MapMultiSampleIdToIndex(osc.high.multiSampleId);
	}

	if(this->CheckIfSoundExists(newSound))
	{
		stdOut << u8"-Skipping sound '" << soundEntry.name << u8"' because it already exists" << endl;
		return true;
	}

	this->targetSet.soundBanks[slot->bankNumber].SetObject(soundEntry.name, slot->pos, new SoundObject(Move(newSound)));
	this->importedSounds.Insert(programChangeSequence, Set::CreateRAMSoundProgramChangeSequence(slot->bankNumber, slot->pos));

	stdOut << u8"-Successfully imported sound '" << soundEntry.name << u8"' to slot: " << slot->bankNumber.ToString() << u8", " << slot->pos << endl;
	return true;
}

bool ResourceImporter::ImportSoundDependencies(const Sound::SoundData& soundData)
{
	for(const auto& osc : soundData.oscillators)
	{
		if(osc.low.source == Sound::MultiSampleSource::RAM)
		{
			if(!this->ImportMultiSample(osc.low.multiSampleId))
				return false;
		}
		if(osc.high.source == Sound::MultiSampleSource::RAM)
		{
			if(!this->ImportMultiSample(osc.high.multiSampleId))
				return false;
		}
	}
	ASSERT_EQUALS(false, soundData.drumKitData.HasValue()); //TODO: implement importing drum kits
	return true;
}

void ResourceImporter::SaveChanges()
{
	this->targetSet.Save();

	uint32 sampleRamSize = this->targetSet.ComputeUsedSampleRAMSize();
	stdOut << u8"Saved." << endl
		   << u8"Sample RAM usage:" << endl
		   << u8"Occupied: " << String::FormatBinaryPrefixed(sampleRamSize) << endl
		   << u8"Free: " << String::FormatBinaryPrefixed(this->targetSet.model.GetSampleRAMSize() * MiB - sampleRamSize) << endl;
}

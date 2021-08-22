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
	const PerformanceObject& performanceObject = performanceEntry.Object();

	stdOut << u8"Importing performance: " << performanceEntry.Name() << endl;

	BankSlot<PerformanceBankNumber> sourceSlot = {bankNumber, pos};
	const auto slot = this->targetSet.performanceBanks.FindFreeSlot(this->config.performanceInsertSlot.HasValue() ? this->config.performanceInsertSlot.Value() : sourceSlot);
	if(!slot.HasValue())
	{
		stdOut << u8"No free performance slot could be found." << endl;
		return;
	}

	bool success;
	switch(performanceObject.Version())
	{
		case 0:
			success = this->ImportPerformanceData(performanceObject.V0Data(), *slot, performanceEntry.Name());
			break;
		case 1:
			success = this->ImportPerformanceData(performanceObject.V1Data(), *slot, performanceEntry.Name());
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	if(success)
	{
		stdOut << u8"Successfully imported performance '" << performanceEntry.Name() << u8"' to slot: " << slot->bankNumber.ToString() << u8", " << slot->pos << endl;

		this->SaveChanges();
	}
}

void ResourceImporter::ImportStyle(const StyleBankNumber &bankNumber, uint8 pos, bool ignoreSTSSounds)
{
	const auto& styleEntry = this->sourceSet.styleBanks[bankNumber][pos];
	const FullStyle& fullStyle = styleEntry.Object();

	stdOut << u8"Importing style: " << styleEntry.Name() << endl;

	BankSlot<StyleBankNumber> sourceSlot = {bankNumber, pos};
	const auto slot = this->targetSet.styleBanks.FindFreeSlot(this->config.styleInsertSlot.HasValue() ? this->config.styleInsertSlot.Value() : sourceSlot);
	if(!slot.HasValue())
	{
		stdOut << u8"No free style slot could be found." << endl;
		return;
	}

	const auto& styleData = fullStyle.Style().data;
	for(const auto& styleElement : styleData.styleElements)
	{
		for(const auto& styleTrackData : styleElement.styleTrackData)
		{
			if(!this->ImportSound(styleTrackData.soundProgramChangeSeq))
				return;
		}
	}
	for(const auto& styleElement : styleData.variation)
	{
		for(const auto& styleTrackData : styleElement.styleTrackData)
		{
			if(!this->ImportSound(styleTrackData.soundProgramChangeSeq))
				return;
		}
	}

	Style::StyleData newStyleData = styleData;
	for(auto& styleElement : newStyleData.styleElements)
	{
		for(auto& styleTrackData : styleElement.styleTrackData)
			styleTrackData.soundProgramChangeSeq = this->MapSoundProgramChangeSequence(styleTrackData.soundProgramChangeSeq);
	}
	for(auto& styleElement : newStyleData.variation)
	{
		for(auto& styleTrackData : styleElement.styleTrackData)
			styleTrackData.soundProgramChangeSeq = this->MapSoundProgramChangeSequence(styleTrackData.soundProgramChangeSeq);
	}

	stdOut << u8"Importing STS: " << styleEntry.Name() << endl;

	UniquePointer<SingleTouchSettings> sts = this->ImportSTS(fullStyle.STS(), ignoreSTSSounds);
	if(sts.IsNull())
		sts = new SingleTouchSettings(new Performance::V1::STSData);

	this->targetSet.styleBanks[slot->bankNumber].SetObject(styleEntry.Name(), slot->pos, new FullStyle(new StyleObject(Move(newStyleData)), Move(sts)));
	stdOut << u8"Successfully imported style '" << styleEntry.Name() << u8"' to slot: " << slot->bankNumber.ToString() << u8", " << slot->pos << endl;

	this->SaveChanges();
}

//Private methods
Optional<BankSlot<SoundBankNumber>> ResourceImporter::FindSoundLocation(const Sound::SoundData& soundData)
{
	for(const auto& bankEntry : this->targetSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(objectEntry.Object().data == soundData)
				return {{.bankNumber = bankEntry.bankNumber, .pos = objectEntry.pos}};
		}
	}
	return {};
}

bool ResourceImporter::ImportMultiSample(uint64 id)
{
	if(this->importedMultiSampleIds.Contains(id))
		return true;
	if(this->targetMultiSamplesIndex.HasMultiSampleEntry(id))
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
				keyboardZone.sampleNumber = this->MapSampleIdToIndex(this->sourceSet.MultiSamples().data.sampleEntries[srcKeyZone.sampleNumber].id);

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
	if(this->targetMultiSamplesIndex.HasSampleEntry(id))
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
	uint32 requiredSize = this->targetSet.ComputeUsedSampleRAMSize() + entry.Object().GetSize();
	uint32 availableSize = this->targetSet.model.GetSampleRAMSize() * MiB;
	if(requiredSize > availableSize)
	{
		stdOut << u8"Sample RAM is full" << endl;
		return false;
	}

	this->targetSet.sampleBanks[slot->bankNumber].SetObject(entry.Name(), slot->pos, entry.Object().Clone());

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
	const SoundObject& soundObject = soundEntry.Object();

	stdOut << u8"-Importing sound: " << soundEntry.Name() << endl;
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

	auto findResult = this->FindSoundLocation(newSound);
	if(findResult.HasValue())
	{
		stdOut << u8"-Skipping sound '" << soundEntry.Name() << u8"' because it already exists" << endl;
		this->importedSounds.Insert(programChangeSequence, Set::CreateRAMSoundProgramChangeSequence(findResult->bankNumber, findResult->pos));
		return true;
	}

	this->targetSet.soundBanks[slot->bankNumber].SetObject(soundEntry.Name(), slot->pos, new SoundObject(Move(newSound)));
	this->importedSounds.Insert(programChangeSequence, Set::CreateRAMSoundProgramChangeSequence(slot->bankNumber, slot->pos));

	stdOut << u8"-Successfully imported sound '" << soundEntry.Name() << u8"' to slot: " << slot->bankNumber.ToString() << u8", " << slot->pos << endl;
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

UniquePointer<SingleTouchSettings> ResourceImporter::ImportSTS(const SingleTouchSettings& sts, bool ignoreSTSSounds)
{
	switch(sts.Version())
	{
		case 0:
			return this->ImportSTSData(sts.V0Data(), ignoreSTSSounds);
		case 1:
			return this->ImportSTSData(sts.V1Data(), ignoreSTSSounds);
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
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
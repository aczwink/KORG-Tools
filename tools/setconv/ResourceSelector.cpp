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
#include "ResourceSelector.hpp"

//Public methods
void ResourceSelector::PrintResults()
{
	stdOut << u8"Selection results: " << endl;
	stdOut << u8"Performances: " << endl;
	for(const auto& bankEntry : this->sourceSet.performanceBanks.Entries())
	{
		uint32 nEntries = 0;
		uint32 nMissingEntries = 0;

		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			nEntries++;
			if(!this->selection.performances.Contains(objectEntry.object.operator->()))
				nMissingEntries++;
		}

		stdOut << bankEntry.bankNumber.ToString() << u8": " << (nEntries - nMissingEntries) << u8"/" << nEntries << u8" entries selected. " << nMissingEntries << u8" skipped." << endl;
	}

	stdOut << u8"Sounds: " << endl;
	for(const auto& bankEntry : this->sourceSet.soundBanks.Entries())
	{
		uint32 nEntries = 0;
		uint32 nMissingEntries = 0;

		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			nEntries++;
			if(!this->selection.sounds.Contains(Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos)))
				nMissingEntries++;
		}

		stdOut << bankEntry.bankNumber.ToString() << u8": " << (nEntries - nMissingEntries) << u8"/" << nEntries << u8" entries selected. " << nMissingEntries << u8" skipped." << endl;
	}

	uint32 nSamples = 0;
	for(const auto& bankEntry : this->sourceSet.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
			nSamples++;
	}

	stdOut << u8"Sample usage: " << this->selection.samples.GetNumberOfElements() << u8"/" << nSamples << endl;
	stdOut << u8"Sample RAM usage: " << String::FormatBinaryPrefixed(this->ComputeUsedSampleRAMSize()) << endl;
}

void ResourceSelector::Select()
{
	this->SelectPerformances();
	this->SelectSounds();
}

//Private methods
bool ResourceSelector::SelectMultiSample(const MultiSamples::MultiSampleEntry &multiSampleEntry)
{
	if(this->selection.multiSamples.Contains(multiSampleEntry.id))
		return true;

	for(uint8 relativeIndex : multiSampleEntry.keyZoneIndex)
	{
		if(relativeIndex == Unsigned<uint8>::Max())
			continue;

		int16 sampleNumber = this->sourceSet.MultiSamples().data.keyboardZones[multiSampleEntry.keyZoneBaseIndex + relativeIndex].sampleNumber;
		if(sampleNumber == -1)
			continue;

		if(!this->SelectPCMSample(this->sourceSet.MultiSamples().data.sampleEntries[sampleNumber].id))
			return false;
	}

	this->selection.multiSamples.Insert(multiSampleEntry.id);
	return true;
}

bool ResourceSelector::SelectPCMSample(const MultiSamples::SampleEntry &sampleEntry)
{
	if(this->selection.samples.Contains(sampleEntry.id))
		return true;

	uint32 targetSampleRAMSize = this->targetSet.model.GetSampleRAMSize() * MiB;
	if(this->ComputeUsedSampleRAMSize() + this->ComputeTargetSampleSize(sampleEntry.id) > targetSampleRAMSize)
		return false;

	this->selection.samples.Insert(sampleEntry.id);
	return true;
}

bool ResourceSelector::SelectPerformance(const PerformanceObject& performanceObject)
{
	if(!this->HasAvailablePerformanceSlot())
		return false;

	switch(performanceObject.Version())
	{
		case 0:
		{
			for(const auto& trackSettings : performanceObject.V0Data().keyboardSettings.trackSettings)
			{
				if(!this->SelectSound(trackSettings.soundProgramChangeSeq))
					return false;
			}
		}
		break;
		case 1:
		{
			for(const auto& trackSettings : performanceObject.V1Data().keyboardSettings.trackSettings)
			{
				if(!this->SelectSound(trackSettings.soundProgramChangeSeq))
					return false;
			}
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	this->selection.performances.Insert(&performanceObject);
	return true;
}

void ResourceSelector::SelectPerformances()
{
	for(const auto& bankEntry : this->sourceSet.performanceBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			this->SelectPerformance(*objectEntry.object);
		}
	}
}

void ResourceSelector::SelectSounds()
{
	for(const auto& bankEntry : this->sourceSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			this->SelectSound(Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos));
		}
	}
}

bool ResourceSelector::SelectSound(const ProgramChangeSequence &programChangeSequence)
{
	if(!Set::IsRAMSound(programChangeSequence, this->targetSet.model))
		return true; //ROM sounds are assumed to be there
	if(this->selection.sounds.Contains(programChangeSequence))
		return true;

	const auto& location = this->setIndex.GetSoundLocation(programChangeSequence);
	if(!location.HasValue())
	{
		if((this->selection.requireFreeSoundSlot == false) and !this->HasAvailableSoundSlot())
			return false;
		this->selection.requireFreeSoundSlot = true; //it is totally valid for a performance to have for example upper2 refer to a free sound while, upper1 is set
		return true;
	}

	if(!this->HasAvailableSoundSlot())
		return false;

	const auto& entry = this->sourceSet.soundBanks[location->bankNumber][location->pos];
	const auto& soundData = entry.object->data;

	ASSERT_EQUALS(false, soundData.drumKitData.HasValue()); //TODO: implement integrating drum samples and drum kit data

	for(const auto& osc : soundData.oscillators)
	{
		if((osc.high.source == libKORG::Sound::MultiSampleSource::RAM) && !this->SelectMultiSample(osc.high.multiSampleId))
			return false;
		if((osc.low.source == libKORG::Sound::MultiSampleSource::RAM) and !this->SelectMultiSample(osc.low.multiSampleId))
			return false;
	}

	this->selection.sounds.Insert(programChangeSequence);
	return true;
}

uint32 ResourceSelector::ComputeTargetSampleSize(uint64 sampleId) const
{
	auto location = this->setIndex.GetSampleLocation(sampleId);
	uint32 sampleSize = this->sourceSet.sampleBanks[location.bankNumber][location.pos].object->GetSize();
	if((this->multiSamplesIndex.GetSampleEntryById(sampleId).packedData.SampleType() == libKORG::MultiSamples::SampleType::Compressed) and !this->targetSet.model.IsSampleCompressionSupported())
		sampleSize *= 2;
	return sampleSize;
}

uint32 ResourceSelector::ComputeUsedSampleRAMSize() const
{
	uint32 totalSize = 0;
	for(uint64 sampleId : this->selection.samples)
		totalSize += this->ComputeTargetSampleSize(sampleId);
	return totalSize;
}

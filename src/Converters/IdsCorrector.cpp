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
#include <libkorg/Converters/IdsCorrector.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
void IdsCorrector::Correct()
{
	const auto& sampleEntries = this->set.MultiSamples().data.sampleEntries;
	for(uint32 i = 0; i < sampleEntries.GetNumberOfElements(); i++)
	{
		const auto& sampleEntry = sampleEntries[i];
		ASSERT(sampleEntry.id != 0, u8"Invalid Ids are not allowed");
		ASSERT_EQUALS(false, this->sampleIdToIndexMap.Contains(sampleEntry.id));
		this->sampleIdToIndexMap.Insert(sampleEntry.id, i);
	}

	/*const auto& drumSampleEntries = this->set.MultiSamples().data.drumSampleEntries;
	stdOut << u8"DRUM SAMPLES: " << drumSampleEntries.GetNumberOfElements() << endl;
	for(uint32 i = 0; i < drumSampleEntries.GetNumberOfElements(); i++)
	{
		const auto& drumSampleEntry = drumSampleEntries[i];

		stdOut << drumSampleEntry.name << " " << drumSampleEntry.id << endl;
	}*/

	this->CorrectSamples();

	const auto& msEntries = this->set.MultiSamples().data.multiSampleEntries;
	for(uint32 i = 0; i < msEntries.GetNumberOfElements(); i++)
	{
		const auto& msEntry = msEntries[i];

		ASSERT(msEntry.id != 0, u8"Invalid Ids are not allowed");
		ASSERT_EQUALS(false, this->multiSampleIdToIndexMap.Contains(msEntry.id));
		this->multiSampleIdToIndexMap.Insert(msEntry.id, i);

		for(uint8 relativeIndex : msEntry.keyZoneIndex)
		{
			if(relativeIndex == Unsigned<uint8>::Max())
				continue;

			const auto& keyZone = this->set.MultiSamples().data.keyboardZones[msEntry.keyZoneBaseIndex + relativeIndex];
			if(keyZone.sampleNumber == -1)
				continue;

			ASSERT(keyZone.sampleNumber < sampleEntries.GetNumberOfElements(), u8"Sample number out of bounds");
		}
	}

	for(const auto& bankEntry : this->set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			Sound::SoundData& data = this->set.soundBanks[bankEntry.bankNumber][objectEntry.pos].object->data;
			for(auto& osc : data.oscillators)
			{
				if(osc.high.source == Sound::MultiSampleSource::RAM)
					this->CorrectMultiSampleAssignment(osc.high);
				if(osc.low.source == Sound::MultiSampleSource::RAM)
					this->CorrectMultiSampleAssignment(osc.low);
			}

			ASSERT_EQUALS(false, objectEntry.object->data.drumKitData.HasValue()); //TODO: implement for drumkits
		}
	}
}

//Private methods
void IdsCorrector::CorrectMultiSampleAssignment(Sound::OSCMultiSampleSettings &multiSampleSettings)
{
	if(!this->multiSampleIdToIndexMap.Contains(multiSampleSettings.multiSampleId))
	{
		//keyboard reports "missing items" and sets the source to ROM and the sample to "Empty"
		multiSampleSettings.multiSampleNumber = Unsigned<uint16>::Max();
		multiSampleSettings.multiSampleId = 0;
		multiSampleSettings.source = Sound::MultiSampleSource::ROM;

		this->errorCounter++;
		return;
	}

	uint32 indexById = this->multiSampleIdToIndexMap.Get(multiSampleSettings.multiSampleId);
	if(multiSampleSettings.multiSampleNumber != indexById)
	{
		//this case was verified on the keyboard
		multiSampleSettings.multiSampleNumber = indexById;
	}
}

void IdsCorrector::CorrectSamples()
{
	DynamicArray<Tuple<uint8, uint8>> toDelete;
	BinaryTreeMap<uint64, BankSlot<SampleBankNumber>> foundSamples;

	for(const auto& bankEntry : this->set.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			ASSERT(objectEntry.object->GetId() != 0, u8"Invalid Ids are not allowed");
			if(!this->sampleIdToIndexMap.Contains(objectEntry.object->GetId()))
			{
				toDelete.Push({bankEntry.bankNumber.Number(), objectEntry.pos});
			}

			if(foundSamples.Contains(objectEntry.object->GetId()))
			{
				const auto& entry = foundSamples.Get(objectEntry.object->GetId());
				if(*this->set.sampleBanks[entry.bankNumber][entry.pos].object == *objectEntry.object)
				{
					toDelete.Push({bankEntry.bankNumber.Number(), objectEntry.pos});
				}
				else
				{
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				}
			}
			else
				foundSamples.Insert(objectEntry.object->GetId(), {bankEntry.bankNumber, objectEntry.pos});
		}
	}

	this->errorCounter += toDelete.GetNumberOfElements();
	for(const auto& t : toDelete)
	{
		this->set.sampleBanks[SampleBankNumber(t.Get<0>())].RemoveObject(t.Get<1>());
	}

	const auto& sampleEntries = this->set.MultiSamples().data.sampleEntries;
	for(uint32 i = 0; i < sampleEntries.GetNumberOfElements();)
	{
		const auto& sampleEntry = sampleEntries[i];
		if(foundSamples.Contains(sampleEntry.id))
			i++;
		else
		{
			this->RemoveSampleFromIndex(i);
			this->errorCounter++;
		}
	}
}

void IdsCorrector::RemoveSampleFromIndex(uint32 index)
{
	auto& sampleEntries = this->set.MultiSamples().data.sampleEntries;
	for(uint32 i = index + 1; i < sampleEntries.GetNumberOfElements(); i++)
		sampleEntries[i-1] = sampleEntries[i];
	sampleEntries.Resize(sampleEntries.GetNumberOfElements() - 1);

	auto& keyboardZones = this->set.MultiSamples().data.keyboardZones;
	for(auto& zone : keyboardZones)
	{
		if(zone.sampleNumber == -1)
			continue;
		else if(zone.sampleNumber == index)
			zone.sampleNumber = -1;
		else if(zone.sampleNumber > index)
			zone.sampleNumber--;
	}
}
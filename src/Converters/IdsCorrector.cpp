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
//Local
#include <libkorg/Converters/SampleRemover.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
void IdsCorrector::Correct()
{
	this->CorrectSamples();

	this->BuildDrumSampleIndex();
	this->CorrectDrumSampleAssignments();

	this->BuildMultiSampleIndex();
	this->CorrectMultiSampleAssignments();

	this->RenumberDrumSampleAssignments();
}

//Private methods
void IdsCorrector::BuildDrumSampleIndex()
{
	const auto& sampleEntries = this->set.MultiSamples().data.sampleEntries;
	const auto& drumSampleEntries = this->set.MultiSamples().data.drumSampleEntries;
	//stdOut << u8"DRUM SAMPLES: " << drumSampleEntries.GetNumberOfElements() << endl;
	for(uint32 i = 0; i < drumSampleEntries.GetNumberOfElements(); i++)
	{
		const auto& drumSampleEntry = drumSampleEntries[i];

		ASSERT(drumSampleEntry.id != 0, u8"Invalid Ids are not allowed");
		ASSERT_EQUALS(false, this->drumSampleIdToIndexMap.Contains(drumSampleEntry.id));
		this->drumSampleIdToIndexMap.Insert(drumSampleEntry.id, i);
		//stdOut << drumSampleEntry.name << " " << drumSampleEntry.id << endl;
	}
}

void IdsCorrector::BuildMultiSampleIndex()
{
	const auto& sampleEntries = this->set.MultiSamples().data.sampleEntries;
	const auto& msEntries = this->set.MultiSamples().data.multiSampleEntries;
	for(uint32 i = 0; i < msEntries.GetNumberOfElements(); i++)
	{
		const auto& msEntry = msEntries[i];

		ASSERT(msEntry.id != 0, u8"Invalid Ids are not allowed");
		ASSERT(!this->multiSampleIdToIndexMap.Contains(msEntry.id), u8"Duplicate multi sample (at least same id)");
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
}

StdXX::BinaryTreeMap<uint64, uint32> IdsCorrector::BuildSampleIndex()
{
	StdXX::BinaryTreeMap<uint64, uint32> sampleIdToIndexMap;

	const auto& sampleEntries = this->set.MultiSamples().data.sampleEntries;
	for(uint32 i = 0; i < sampleEntries.GetNumberOfElements(); i++)
	{
		const auto& sampleEntry = sampleEntries[i];
		ASSERT(sampleEntry.id != 0, u8"Invalid Ids are not allowed");
		ASSERT(!sampleIdToIndexMap.Contains(sampleEntry.id), u8"Duplicate sample entry (at least same id)");
		sampleIdToIndexMap.Insert(sampleEntry.id, i);
	}

	return sampleIdToIndexMap;
}

void IdsCorrector::CorrectDrumSampleAssignment(Sound::LayerEntry& layer)
{
	if(!this->drumSampleIdToIndexMap.Contains(layer.drumSampleId))
	{
		layer.drumSampleNumber = Unsigned<uint16>::Max();
		layer.drumSampleId = 0;
		layer.sampleBankNumber = 0;
		this->errorCounts.missingDrumSamplesCount++;
		return;
	}

	uint32 indexById = this->drumSampleIdToIndexMap.Get(layer.drumSampleId);
	layer.drumSampleNumber = indexById;
}

void IdsCorrector::CorrectDrumSampleAssignments()
{
	for(const auto& bankEntry : this->set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(objectEntry.Object().data.drumKitData.HasValue())
			{
				Sound::SoundData& data = this->set.soundBanks[bankEntry.bankNumber][objectEntry.pos].Object().data;
				auto& layers = data.drumKitData->layers;
				for(auto& layer : layers)
				{
					if(layer.sampleBankNumber > 0)
						this->CorrectDrumSampleAssignment(layer);
				}
			}
		}
	}
}

void IdsCorrector::CorrectMultiSampleAssignment(Sound::OSCMultiSampleSettings &multiSampleSettings)
{
	if(!this->multiSampleIdToIndexMap.Contains(multiSampleSettings.multiSampleId))
	{
		//keyboard reports "missing items" and sets the source to ROM and the sample to "Empty"
		multiSampleSettings.multiSampleNumber = Unsigned<uint16>::Max();
		multiSampleSettings.multiSampleId = 0;
		multiSampleSettings.source = Sound::MultiSampleSource::ROM;

		this->errorCounts.missingSamplesCount++;
		return;
	}

	uint32 indexById = this->multiSampleIdToIndexMap.Get(multiSampleSettings.multiSampleId);
	if(multiSampleSettings.multiSampleNumber != indexById)
	{
		//this case was verified on the keyboard
		multiSampleSettings.multiSampleNumber = indexById;
	}
}

void IdsCorrector::CorrectMultiSampleAssignments()
{
	for(const auto& bankEntry : this->set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			Sound::SoundData& data = this->set.soundBanks[bankEntry.bankNumber][objectEntry.pos].Object().data;
			for(auto& osc : data.oscillators)
			{
				if(osc.high.source == Sound::MultiSampleSource::RAM)
					this->CorrectMultiSampleAssignment(osc.high);
				if(osc.low.source == Sound::MultiSampleSource::RAM)
					this->CorrectMultiSampleAssignment(osc.low);
			}
		}
	}
}

void IdsCorrector::CorrectSamples()
{
	DynamicArray<Tuple<uint8, uint8>> toDelete;
	BinaryTreeMap<uint64, BankSlot<SampleBankNumber>> foundSamples;

	const auto sampleIdToIndexMap = this->BuildSampleIndex();
	for(const auto& bankEntry : this->set.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			ASSERT(objectEntry.Object().GetId() != 0, u8"Invalid Ids are not allowed");
			if(!sampleIdToIndexMap.Contains(objectEntry.Object().GetId()))
			{
				toDelete.Push({bankEntry.bankNumber.Number(), objectEntry.pos});
				this->errorCounts.missingSamplesCount++;
			}
			else if(foundSamples.Contains(objectEntry.Object().GetId()))
			{
				const auto& entry = foundSamples.Get(objectEntry.Object().GetId());
				if(this->set.sampleBanks[entry.bankNumber][entry.pos].Object() == objectEntry.Object()) //duplicate
				{
					toDelete.Push({bankEntry.bankNumber.Number(), objectEntry.pos});
				}
				else
				{
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				}
			}
			else
				foundSamples.Insert(objectEntry.Object().GetId(), {bankEntry.bankNumber, objectEntry.pos});
		}
	}

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
			SampleRemover sampleRemover(this->set.MultiSamples().data);
			sampleRemover.RemoveSampleFromIndex(i);
			this->errorCounts.missingSamplesCount++;
		}
	}
}

void IdsCorrector::RenumberDrumSampleAssignments()
{
	for(const auto& bankEntry : this->set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(objectEntry.Object().data.drumKitData.HasValue())
			{
				Sound::SoundData& data = this->set.soundBanks[bankEntry.bankNumber][objectEntry.pos].Object().data;
				for(Sound::LayerEntry& layer : data.drumKitData->layers)
				{
					if(layer.sampleBankNumber > 0)
						layer.drumSampleNumber = this->drumSampleIdToIndexMap.Get(layer.drumSampleId);
				}
			}
		}
	}
}
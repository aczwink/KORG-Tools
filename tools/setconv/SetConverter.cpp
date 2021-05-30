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
#include "SetConverter.hpp"
//Local
#include "ResourceSelector.hpp"
#include "BankAllocator.hpp"

void SetConverter::Convert()
{
	stdOut << u8"Selecting resources..." << endl;
	ResourceSelector selector(this->sourceSet, this->targetSet, this->setIndex, this->multiSamplesIndex);
	selector.Select();
	selector.PrintResults();

	stdOut << u8"Mapping resources to bank locations..." << endl;
	BankAllocator bankAllocator(this->sourceSet, this->targetSet);
	bankAllocator.Compute(selector.Selection());

	stdOut << u8"Converting resources..." << endl;

	this->IntegratePerformances(bankAllocator.Allocation().performanceAllocation);

	stdOut << u8"Saving..." << endl;
	this->targetSet.Save();

	stdOut << u8"Done." << endl;

	NOT_IMPLEMENTED_ERROR; //TODO: implement me

	this->IntegrateSounds();
	this->IntegrateMultiSamples();
	this->IntegratePCM();

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
uint32 SetConverter::ComputeSampleSize(const Sample::SampleData& sampleData)
{
	return sampleData.sampleBuffer.Size();
}

Sample::SampleData SetConverter::ConvertSampleIfRequired(const Sample::SampleData& sampleData)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	//if ((sampleData.sampleFormat == Sample::SampleFormat::Compressed) and !targetModel.IsSampleCompressionSupported())
	{
		Sample::SampleData convertedData = sampleData;
		convertedData.sampleFormat = Sample::SampleFormat::Linear_PCM_S16BE;

		const auto& entry = this->multiSamplesIndex.GetSampleEntryById(sampleData.id);

		Multimedia::AudioBuffer* audioBuffer = new Multimedia::AudioBuffer(sampleData.nSamples, Multimedia::AudioSampleFormat(1, Multimedia::AudioSampleType::S16, false));
		Sample::Decompress(sampleData.sampleBuffer.Data(), static_cast<int16 *>(audioBuffer->GetPlane(0)), sampleData.nSamples, entry->compressionCoefficients[0], entry->compressionCoefficients[1]);
		Multimedia::AudioFrame frame(audioBuffer);
		UniquePointer<Multimedia::Packet> packet = this->EncodeAudio(frame, Multimedia::CodingFormatId::PCM_S16BE, sampleData.sampleRate);

		convertedData.sampleBuffer.Resize(packet->GetSize());
		convertedData.sampleBuffer.CopyFrom(packet->GetData(), 0, packet->GetSize());

		return convertedData;
	}

	return sampleData;
}

Multimedia::Packet* SetConverter::EncodeAudio(const Multimedia::Frame& audioFrame, Multimedia::CodingFormatId targetFormatId, uint32 sampleRate) const
{
	UniquePointer<Multimedia::AudioStream> targetStream = new Multimedia::AudioStream;
	targetStream->SetCodingFormat(targetFormatId);
	targetStream->sampleFormat = Multimedia::AudioSampleFormat(1, Multimedia::AudioSampleType::S16, false);
	targetStream->codingParameters.audio.sampleRate = sampleRate;

	UniquePointer<Multimedia::EncoderContext> encoderContext = Multimedia::CodingFormat::GetCodingFormatById(targetFormatId)->GetBestMatchingEncoder()->CreateContext(*targetStream);
	encoderContext->Encode(audioFrame);

	return encoderContext->GetNextPacket();
}

MultiSamples::KeyboardZone SetConverter::MapKeyboardZone(const MultiSamples::KeyboardZone &keyboardZone) const
{
	if(keyboardZone.sampleNumber == -1)
		return keyboardZone;

	MultiSamples::KeyboardZone mapped = keyboardZone;

	uint64 sampleId = this->sourceSet.MultiSamples().data.sampleEntries[keyboardZone.sampleNumber].id;
	for(uint32 i = 0; i < this->targetSet.MultiSamples().data.sampleEntries.GetNumberOfElements(); i++)
	{
		if(this->targetSet.MultiSamples().data.sampleEntries[i].id == sampleId)
		{
			mapped.sampleNumber = i;
			break;
		}
	}

	return mapped;
}

bool SetConverter::IntegrateMultiSample(const MultiSamples::MultiSampleEntry& multiSampleEntry)
{
	if(this->mapped.integratedMultiSampleIds.Contains(multiSampleEntry.id))
		return true;

	//integrate it
	MultiSamples::MultiSampleEntry newMultiSampleEntry = multiSampleEntry;
	newMultiSampleEntry.keyZoneBaseIndex = this->targetSet.MultiSamples().data.keyboardZones.GetNumberOfElements();
	BinaryTreeMap<uint8, uint8> keyZoneIndexMap;
	for(uint8 i = 0; i < 128; i++)
	{
		uint8 relativeIndex = newMultiSampleEntry.keyZoneIndex[i];
		if(relativeIndex == Unsigned<uint8>::Max())
			continue;

		if(!keyZoneIndexMap.Contains(relativeIndex))
		{
			MultiSamples::KeyboardZone keyBoardZone = this->MapKeyboardZone(this->sourceSet.MultiSamples().data.keyboardZones[multiSampleEntry.keyZoneBaseIndex + relativeIndex]);
			uint32 newAbsIndex = this->targetSet.MultiSamples().data.keyboardZones.Push(keyBoardZone);
			uint16 newRelIndex = newAbsIndex - newMultiSampleEntry.keyZoneBaseIndex;
			keyZoneIndexMap.Insert(relativeIndex, newRelIndex);
		}

		newMultiSampleEntry.keyZoneIndex[i] = keyZoneIndexMap.Get(relativeIndex);
	}
	uint32 index = this->targetSet.MultiSamples().data.multiSampleEntries.Push(newMultiSampleEntry);
	this->mapped.integratedMultiSampleIds.Insert(multiSampleEntry.id, index);

	return true;
}

void SetConverter::IntegrateMultiSamples()
{
	for(const auto& ms : this->sourceSet.MultiSamples().data.multiSampleEntries)
	{
		this->IntegrateMultiSample(ms);
	}
}

void SetConverter::IntegratePerformances(const BinaryTreeMap<const PerformanceObject *, Tuple<PerformanceBankNumber, uint8, String>> &performanceAllocation)
{
	for(const auto& kv : performanceAllocation)
	{
		PerformanceConverter converter;
		PerformanceObject converted = converter.Convert(*kv.key, this->targetSet.model.GetSupportedResourceVersions().maxPerformanceVersion.major);
		this->targetSet.performanceBanks[kv.value.Get<0>()].AddObject(kv.value.Get<2>(), kv.value.Get<1>(), new PerformanceObject(Move(converted)));
	}
}

void SetConverter::IntegratePCM()
{
	for(const auto& bankEntry : this->sourceSet.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const AbstractSample& sample = *objectEntry.object;
			const auto& sampleObject = dynamic_cast<const SampleObject&>(sample);

			auto entry = this->multiSamplesIndex.GetSampleEntryById(sampleObject.data.id);
			if(entry)
				this->IntegratePCMSample(*entry);
		}
	}
}

bool SetConverter::IntegratePCMSample(const MultiSamples::SampleEntry& sampleEntry)
{
	if(this->mapped.integratedSampleIds.Contains(sampleEntry.id))
		return true;

	const auto& location = this->setIndex.GetSampleLocation(sampleEntry.id);
	if(!location.HasValue())
	{
		stdOut << u8"Free PCM data for entry: " << sampleEntry.id << u8" (" << sampleEntry.name << u8")" << endl;
		return false;
	}

	const auto& bankNumber = location->Get<0>();
	uint8 pos = location->Get<1>();
	const auto& entry = this->sourceSet.sampleBanks[bankNumber][pos];

	const AbstractSample& sample = *entry.object;
	const auto& sampleObject = dynamic_cast<const SampleObject&>(sample);

	Sample::SampleData convertedSampleData = this->ConvertSampleIfRequired(sampleObject.data);

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	/*
	this->targetSet.sampleBanks[bankId].AddObject(entry.name, pos, new SampleObject(Move(convertedSampleData)));

	MultiSamples::SampleEntry mappedSample = sampleEntry;
	if(!this->targetModel.IsSampleCompressionSupported())
	{
		mappedSample.packedData.SampleType(MultiSamples::SampleType::Linear_PCM_S16BE);
		mappedSample.compressionCoefficients[0] = 0;
		mappedSample.compressionCoefficients[1] = 0;
	}
	this->targetSet.MultiSamples().data.sampleEntries.Push(mappedSample);*/

	this->mapped.integratedSampleIds.Insert(sampleEntry.id);

	return true;
}

bool SetConverter::IntegrateSound(const ProgramChangeSequence& programChangeSequence)
{
	NOT_IMPLEMENTED_ERROR;
	/*
	for(const auto& osc : soundData.oscillators)
	{
		if((osc.high.source == libKORG::Sound::MultiSampleSource::RAM) && !this->IntegrateMultiSample(osc.high.multiSampleId))
		{
			this->ShowMultiSampleErrorMessage(osc.high.multiSampleId);
			return false;
		}

		if((osc.low.source == libKORG::Sound::MultiSampleSource::RAM) && !this->IntegrateMultiSample(osc.low.multiSampleId))
		{
			this->ShowMultiSampleErrorMessage(osc.low.multiSampleId);
			return false;
		}
	}

	Sound::SoundData newSound = soundData;
	for(auto& osc : newSound.oscillators)
	{
		if(osc.high.source == Sound::MultiSampleSource::RAM)
			osc.high.multiSampleNumber = this->mapped.integratedMultiSampleIds.Get(osc.high.multiSampleId);
		if(osc.low.source == Sound::MultiSampleSource::RAM)
			osc.low.multiSampleNumber = this->mapped.integratedMultiSampleIds.Get(osc.low.multiSampleId);
	}

	this->targetSet.soundBanks[bankId].AddObject(entry.name, pos, new SoundObject(Move(newSound)));
*/
	return true;
}

void SetConverter::IntegrateSounds()
{
	for(const auto& bankEntry : this->sourceSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(!this->IntegrateSound(Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos)))
			{
				stdOut << u8"Can't integrate sound '" << objectEntry.name << u8" because of missing multisamples." << endl;
			}
		}
	}
}
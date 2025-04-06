/*
 * Copyright (c) 2021-2025 Amir Czwink (amir130@hotmail.de)
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

	this->IntegratePCM(selector.Selection().samples);
	this->IntegrateMultiSamples(selector.Selection().multiSamples);
	this->IntegrateSounds(bankAllocator.Allocation().soundAllocation);

	this->IntegratePerformances(bankAllocator.Allocation().performanceAllocation, bankAllocator.Allocation().freeSoundSpot);

	stdOut << u8"Saving..." << endl;
	this->targetSet.Save();

	stdOut << u8"Done." << endl;
}

//Private methods
Sample::SampleData SetConverter::ConvertSampleIfRequired(const Sample::SampleData& sampleData)
{
	if ((sampleData.sampleFormat == Sample::SampleFormat::Compressed) and !this->targetSet.model.IsSampleCompressionSupported())
	{
		Sample::SampleData convertedData = sampleData;
		convertedData.sampleFormat = Sample::SampleFormat::Linear_PCM_S16BE;

		const auto& entry = this->multiSamplesIndex.GetSampleEntryById(sampleData.id);

		Multimedia::AudioBuffer* audioBuffer = new Multimedia::AudioBuffer(sampleData.nSamples, Multimedia::AudioSampleFormat(1, Multimedia::AudioSampleType::S16, false));
		Sample::Decompress(sampleData.sampleBuffer.Data(), static_cast<int16 *>(audioBuffer->GetPlane(0)), sampleData.nSamples, entry.compressionCoefficients[0], entry.compressionCoefficients[1]);
		Multimedia::Frame frame(audioBuffer);
		UniquePointer<Multimedia::Packet> packet = this->EncodeAudio(frame, Multimedia::CodingFormatId::PCM_S16BE, sampleData.sampleRate);

		convertedData.sampleBuffer.Resize(packet->GetSize());
		convertedData.sampleBuffer.CopyFrom(packet->GetData(), 0, packet->GetSize());

		return convertedData;
	}

	return sampleData;
}

Multimedia::Packet* SetConverter::EncodeAudio(const Multimedia::Frame& audioFrame, Multimedia::CodingFormatId targetFormatId, uint32 sampleRate) const
{
	UniquePointer<Multimedia::Stream> targetStream = new Multimedia::Stream(Multimedia::DataType::Audio);
	targetStream->SetCodingFormat(targetFormatId);
	targetStream->codingParameters.audio.sampleFormat = Multimedia::AudioSampleFormat(1, Multimedia::AudioSampleType::S16, false);
	targetStream->codingParameters.audio.sampleRate = sampleRate;

	UniquePointer<Multimedia::EncoderContext> encoderContext = Multimedia::FormatRegistry::Instance().GetCodingFormatById(targetFormatId)->GetBestMatchingEncoder()->CreateContext(targetStream->codingParameters);
	encoderContext->Encode(audioFrame);

	return encoderContext->GetNextPacket();
}

bool SetConverter::IntegrateMultiSample(const MultiSamples::MultiSampleEntry& multiSampleEntry)
{
	if(this->mapped.integratedMultiSampleIds.Contains(multiSampleEntry.id))
		return true;

	//integrate it
	MultiSamples::MultiSampleEntry newMultiSampleEntry = multiSampleEntry;
	newMultiSampleEntry.keyZoneBaseIndex = this->targetSet.MultiSamples().data.keyboardZones.GetNumberOfElements();
	BinaryTreeSet<uint8> importedKeyZones;
	for(uint8 relativeIndex : newMultiSampleEntry.keyZoneIndex)
	{
		if(relativeIndex == Unsigned<uint8>::Max())
			continue;

		if(!importedKeyZones.Contains(relativeIndex))
		{
			const MultiSamples::MultiSamplesData &multiSamplesData = this->sourceSet.MultiSamples().data;

			MultiSamples::KeyboardZone mappedKeyboardZone = multiSamplesData.keyboardZones[multiSampleEntry.keyZoneBaseIndex + relativeIndex];
			if(mappedKeyboardZone.sampleNumber != -1)
			{
				uint64 sampleId = multiSamplesData.sampleEntries[mappedKeyboardZone.sampleNumber].id;
				mappedKeyboardZone.sampleNumber = this->mapped.integratedSampleIds.Get(sampleId);
			}

			this->targetSet.MultiSamples().data.keyboardZones.Push(mappedKeyboardZone);
			importedKeyZones.Insert(relativeIndex);
		}
	}
	uint32 index = this->targetSet.MultiSamples().data.multiSampleEntries.Push(newMultiSampleEntry);
	this->mapped.integratedMultiSampleIds.Insert(multiSampleEntry.id, index);

	return true;
}

void SetConverter::IntegrateMultiSamples(const BinaryTreeSet<uint64>& selectedMultiSampleIds)
{
	for(const auto& ms : this->sourceSet.MultiSamples().data.multiSampleEntries)
	{
		if(!selectedMultiSampleIds.Contains(ms.id))
			continue;
		this->IntegrateMultiSample(ms);
	}
}

void SetConverter::IntegratePerformances(const BinaryTreeMap<const PerformanceObject *, Tuple<PerformanceBankNumber, uint8, String>> &performanceAllocation, const ProgramChangeSequence& freeSoundSpotProgramChangeSequence)
{
	for(const auto& kv : performanceAllocation)
	{
		PerformanceConverter converter;
		PerformanceObject converted = converter.Convert(*kv.key, this->targetSet.model.GetSupportedResourceVersions().maxPerformanceVersion.major);

		switch(converted.Version())
		{
			case 1:
			{
				for(auto& trackSettings : converted.V1Data().keyboardSettings.trackSettings)
				{
					if(Set::IsRAMSound(trackSettings.soundProgramChangeSeq, this->sourceSet.model))
					{
						if(this->mapped.integratedSounds.Contains(trackSettings.soundProgramChangeSeq))
							trackSettings.soundProgramChangeSeq = this->mapped.integratedSounds.Get(trackSettings.soundProgramChangeSeq);
						else
							trackSettings.soundProgramChangeSeq = freeSoundSpotProgramChangeSequence;
					}
				}
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: map program change sequences
		}

		this->targetSet.performanceBanks[kv.value.Get<0>()].SetObject(kv.value.Get<2>(), kv.value.Get<1>(),
																	  new PerformanceObject(Move(converted)));
	}
}

void SetConverter::IntegratePCM(const BinaryTreeSet<uint64>& selectedSampleIds)
{
	for(const auto& bankEntry : this->sourceSet.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(!selectedSampleIds.Contains(objectEntry.Object().GetId()))
				continue;

			const AbstractSample& sample = objectEntry.Object();
			const auto& sampleObject = dynamic_cast<const SampleObject&>(sample);

			auto entry = this->multiSamplesIndex.GetSampleEntryById(sampleObject.data.id);
			this->IntegratePCMSample(entry);
		}
	}
}

bool SetConverter::IntegratePCMSample(const MultiSamples::SampleEntry& sampleEntry)
{
	const auto& location = this->setIndex.GetSampleLocation(sampleEntry.id);

	const auto& entry = this->sourceSet.sampleBanks[location.bankNumber][location.pos];

	const AbstractSample& sample = entry.Object();
	const auto& sampleObject = dynamic_cast<const SampleObject&>(sample);

	Sample::SampleData convertedSampleData = this->ConvertSampleIfRequired(sampleObject.data);

	this->targetSet.sampleBanks[location.bankNumber].SetObject(entry.Name(), location.pos,
																new SampleObject(Move(convertedSampleData)));

	MultiSamples::SampleEntry mappedSample = sampleEntry;
	if(!this->targetSet.model.IsSampleCompressionSupported())
	{
		mappedSample.packedData.SampleType(MultiSamples::SampleType::Linear_PCM_S16BE);
		mappedSample.compressionCoefficients[0] = 0;
		mappedSample.compressionCoefficients[1] = 0;
	}

	uint32 index = this->targetSet.MultiSamples().data.sampleEntries.Push(mappedSample);
	this->mapped.integratedSampleIds.Insert(sampleEntry.id, index);

	return true;
}

void SetConverter::IntegrateSounds(const BinaryTreeMap<ProgramChangeSequence, Tuple<SoundBankNumber, uint8, String>>& soundAllocation)
{
	for(const auto& kv : soundAllocation)
	{
		auto slot = this->setIndex.GetSoundLocation(kv.key);
		const auto& soundObject = this->sourceSet.soundBanks[slot->bankNumber][slot->pos].Object();

		Sound::SoundData newSound = soundObject.data;

		for(auto& osc : newSound.oscillators)
		{
			if(osc.high.source == Sound::MultiSampleSource::RAM)
				osc.high.multiSampleNumber = this->MapMultiSampleIdToIndex(osc.high.multiSampleId);
			if(osc.low.source == Sound::MultiSampleSource::RAM)
				osc.low.multiSampleNumber = this->MapMultiSampleIdToIndex(osc.low.multiSampleId);
		}

		SoundBankNumber bankNumber = kv.value.Get<0>();
		String name = kv.value.Get<2>();
		uint8 pos = kv.value.Get<1>();

		this->targetSet.soundBanks[bankNumber].SetObject(name, pos, new SoundObject(Move(newSound)));
		this->mapped.integratedSounds[kv.key] = Set::CreateRAMSoundProgramChangeSequence(bankNumber, pos);
	}
}
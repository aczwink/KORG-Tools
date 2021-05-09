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

void SetConverter::Convert()
{
	DynamicArray<const SoundObject*> selectedSounds = this->SelectSounds(sourceSet);
	SoundSelection soundSelection(sourceSet, selectedSounds);

	this->IntegratePCM(sourceSet, soundSelection);

	for(const auto& bankEntry : sourceSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(selectedSounds.Find(objectEntry.object.operator->()) != Unsigned<uint32>::Max())
			{
				targetSet.soundBanks[bankEntry.bankNumber].AddObject(objectEntry.name, objectEntry.pos, objectEntry.object);
			}
			else
			{
				stdOut << u8"Can't convert sound '" << objectEntry.name << u8"'. Sample RAM is full." << endl;
			}
		}
	}
	targetSet.Save(targetModel);
}

//Private methods
uint32 SetConverter::ComputeRequiredSampleRAMSize(const Set& set, const SoundSelection& selection)
{
	uint32 sum = 0;
	for(const auto& bankEntry : set.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const AbstractSample* sample = objectEntry.object.operator->();
			const auto* sampleObject = dynamic_cast<const SampleObject*>(sample);
			if(sampleObject and selection.SampleIds().Contains(sampleObject->data.id))
			{
				sum += this->ComputeTargetSampleSize(sampleObject->data);
			}
		}
	}

	return sum;
}

uint32 SetConverter::ComputeTargetSampleSize(const Sample::SampleData& sampleData)
{
	if((sampleData.sampleFormat == Sample::SampleFormat::Compressed) and !targetModel.IsSampleCompressionSupported())
		return sampleData.sampleBuffer.Size() * 2;

	return sampleData.sampleBuffer.Size();
}

Sample::SampleData SetConverter::ConvertSampleIfRequired(const Sample::SampleData& sampleData)
{
	if ((sampleData.sampleFormat == Sample::SampleFormat::Compressed) and !targetModel.IsSampleCompressionSupported())
	{
		Sample::SampleData convertedData = sampleData;
		convertedData.sampleFormat = Sample::SampleFormat::Linear_PCM_S16BE;

		Multimedia::AudioBuffer audioBuffer(sampleData.nSamples, Multimedia::AudioSampleFormat(1, Multimedia::AudioSampleType::S16, false));
		Sample::Decompress(sampleData.sampleBuffer.Data(), static_cast<int16 *>(audioBuffer.GetPlane(0)), sampleData.nSamples);
		Multimedia::AudioFrame frame(&audioBuffer);
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

void SetConverter::IntegratePCM(const Set &sourceSet, const SoundSelection& selectedSounds)
{
	for(const auto& bankEntry : sourceSet.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const AbstractSample* sample = objectEntry.object.operator->();
			const auto* sampleObject = dynamic_cast<const SampleObject*>(sample);
			if(sampleObject and selectedSounds.SampleIds().Contains(sampleObject->data.id))
			{
				Sample::SampleData&& convertedSampleData = this->ConvertSampleIfRequired(sampleObject->data);

				this->targetSet.sampleBanks[bankEntry.bankNumber].AddObject(objectEntry.name, objectEntry.pos, new SampleObject(Move(convertedSampleData)));
			}
		}
	}
}

DynamicArray<const SoundObject*> SetConverter::SelectSounds(const Set& set)
{
	uint32 targetSampleRAMSize = targetModel.GetSampleRAMSize() * MiB;

	DynamicArray<const SoundObject*> selectedObjects;
	for(const auto& bankEntry : set.soundBanks.Entries())
	{
		for(const auto& objectEntry: bankEntry.bank.Objects())
		{
			DynamicArray<const SoundObject*> newSelection = selectedObjects;
			const SoundObject* sound = objectEntry.object.operator->();
			newSelection.Push(sound);
			SoundSelection soundSelection(set, newSelection);

			if(this->ComputeRequiredSampleRAMSize(set, soundSelection) <= targetSampleRAMSize)
				selectedObjects.Push(sound);
		}
	}

	return selectedObjects;
}
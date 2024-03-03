/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include "VirtualInstrument.hpp"

//Public methods
bool VirtualInstrument::CanDrumKeySound(const Pitch &pitch) const
{
	const auto& drumKitSoundData = this->soundData.drumKitData.Value();
	const Sound::KeyTableEntry& keyTableEntry = drumKitSoundData.keyTable[pitch.Encode()];
	const Sound::LayerEntry& layerEntry = drumKitSoundData.layers[keyTableEntry.index];

	//TODO: velocity sample switchting

	if(layerEntry.sampleBankNumber == 0) //ROM samples can't be played
		return false;

	return true;
}

bool VirtualInstrument::CanMultiSampleSound(const Pitch &pitch) const
{
	for(uint32 i = 0; i < this->soundData.oscillators.GetNumberOfElements(); i++)
	{
		const auto& osc = this->soundData.oscillators[i];

		if(this->CanOscillatorSound(pitch, osc.low) or this->CanOscillatorSound(pitch, osc.high))
			return true;
	}

	return false;
}

void VirtualInstrument::NoteOff(const Pitch &pitch)
{
	auto & sources = this->keySources[pitch.Encode()];

	for(auto& src : sources.low)
	{
		if(!src.IsNull())
		{
			src->Stop();
			src->DequeueProcessedBuffers();
		}
	}

	for(auto& src : sources.high)
	{
		if(!src.IsNull())
		{
			src->Stop();
			src->DequeueProcessedBuffers();
		}
	}
}

//Private methods
bool VirtualInstrument::CanOscillatorSound(const Pitch &pitch, const Sound::OSCMultiSampleSettings& osc) const
{
	if(osc.source != Sound::MultiSampleSource::RAM)
		return false;

	const auto& multiSampleEntry = this->playBackSet.GetMultiSampleEntry(osc.multiSampleId);
	uint8 relativeKeyZoneIndex = multiSampleEntry.keyZoneIndex[pitch.Encode()];

	return relativeKeyZoneIndex != Unsigned<uint8>::Max();

}

void VirtualInstrument::NoteOnDrumKit(const Pitch &pitch, uint8 velocity)
{
	uint8 encodedPitch = pitch.Encode();

	const auto& drumKitSoundData = this->soundData.drumKitData.Value();
	const Sound::KeyTableEntry& keyTableEntry = drumKitSoundData.keyTable[encodedPitch];
	const Sound::LayerEntry& layerEntry = drumKitSoundData.layers[keyTableEntry.index];

	//TODO: velocity sample switchting

	if(layerEntry.sampleBankNumber == 0) //ROM samples can't be played
		return;

	const auto& ds = this->playBackSet.GetDrumSample(layerEntry.drumSampleId);
	ASSERT_EQUALS(-1, ds.sampleIndexRight);

	const auto& sampleEntry = this->playBackSet.GetSampleEntryByIndex(ds.sampleIndexLeft);
	Audio::Buffer* buffer = this->playBackFactory.LoadSample(sampleEntry.id);

	auto& oscSources = this->keySources[encodedPitch];
	if(oscSources.low[0].IsNull())
		oscSources.low[0] = this->playBackFactory.CreateSource();
	auto& source = oscSources.low[0];

	source->EnqueueBuffer(*buffer);
	source->Play();
}

void VirtualInstrument::NoteOnMultiSamples(const Pitch &pitch, uint8 velocity)
{
	//TODO: velocity

	if(this->soundData.oscillators.GetNumberOfElements() > c_maxOscillatorsPerSound)
		NOT_IMPLEMENTED_ERROR; //TODO: implement me

	uint8 encodedPitch = pitch.Encode();
	auto& oscSources = this->keySources[encodedPitch];

	for(uint32 i = 0; i < this->soundData.oscillators.GetNumberOfElements(); i++)
	{
		const auto& osc = this->soundData.oscillators[i];

		this->NoteOnOscillator(encodedPitch, osc.low, oscSources.low[i]);
		this->NoteOnOscillator(encodedPitch, osc.high, oscSources.high[i]);
	}
}

void VirtualInstrument::NoteOnOscillator(uint8 encodedPitch, const Sound::OSCMultiSampleSettings& osc, UniquePointer<Audio::Source>& oscSource)
{
	if(osc.source == Sound::MultiSampleSource::RAM)
	{
		const auto& multiSampleEntry = this->playBackSet.GetMultiSampleEntry(osc.multiSampleId);
		uint8 relativeKeyZoneIndex = multiSampleEntry.keyZoneIndex[encodedPitch];
		if(relativeKeyZoneIndex == Unsigned<uint8>::Max())
			return;

		uint16 keyboardZoneIndex = multiSampleEntry.keyZoneBaseIndex + relativeKeyZoneIndex;
		const auto& keyBoardZone = this->playBackSet.GetKeyboardZone(keyboardZoneIndex);
		const auto& sampleEntry = this->playBackSet.GetSampleEntryByIndex(keyBoardZone.sampleNumber);

		Audio::Buffer* buffer = this->playBackFactory.LoadSample(sampleEntry.id);
		if(oscSource.IsNull())
			oscSource = this->playBackFactory.CreateSource();

		oscSource->EnqueueBuffer(*buffer);
		oscSource->Play();
	}
}
/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include "SoftwareSimulatedKORG_MIDI_Synthesizer.hpp"
//Local
#include "SampleLoader.hpp"

//Constructor
SoftwareSimulatedKORG_MIDI_Synthesizer::SoftwareSimulatedKORG_MIDI_Synthesizer(const Set& set) : playBackSet(set)
{
    DeviceEnumerator deviceEnumerator(DeviceType::Audio);
    this->audioDevice = deviceEnumerator.GetNextDevice().MoveCast<AudioDevice>();
    this->deviceContext = this->audioDevice->CreateDeviceContext();
}

//Destructor
SoftwareSimulatedKORG_MIDI_Synthesizer::~SoftwareSimulatedKORG_MIDI_Synthesizer()
{
    for(const auto& kv : this->sampleBuffers)
    {
        delete kv.value;
    }
    this->sampleBuffers.Release();
}

//Public methods
void SoftwareSimulatedKORG_MIDI_Synthesizer::ControlChange(uint32 trackNumber, uint8 controllerNumber, uint8 controllerValue)
{
}

Audio::Source *SoftwareSimulatedKORG_MIDI_Synthesizer::CreateSource()
{
    return this->deviceContext->CreateSource();
}

Audio::Buffer* SoftwareSimulatedKORG_MIDI_Synthesizer::LoadSample(uint64 id)
{
    if(this->sampleBuffers.Contains(id))
        return this->sampleBuffers.Get(id);

    const AbstractSample& sample = this->playBackSet.GetSample(id);
    const SampleObject* sampleObject = dynamic_cast<const SampleObject *>(&sample);

    if(sampleObject)
    {
        SampleLoader sampleLoader;
        UniquePointer<Multimedia::AudioFrame> frame = sampleLoader.LoadSignedPCM16Bit(sampleObject->data, nullptr);
        const Multimedia::AudioBuffer* audioBuffer = frame->GetAudioBuffer();

        Audio::Buffer* buffer = this->deviceContext->CreateBuffer();
        buffer->SetData(const_cast<void *>(audioBuffer->GetPlane(0)), audioBuffer->GetPlaneSize(0), sampleObject->data.sampleRate, 1);
        this->sampleBuffers[id] = buffer;

        return buffer;
    }

    return nullptr;
}

void SoftwareSimulatedKORG_MIDI_Synthesizer::NoteOff(uint32 trackNumber, uint8 note, uint8 velocity)
{
    auto& instrument = this->trackInstruments[trackNumber];
    if(!instrument.IsNull())
        instrument->StopNote(note, velocity);
}

void SoftwareSimulatedKORG_MIDI_Synthesizer::NoteOn(uint32 trackNumber, uint8 note, uint8 velocity)
{
    auto& instrument = this->trackInstruments[trackNumber];
    if(!instrument.IsNull())
        instrument->PlayNote(note, velocity);
}

void SoftwareSimulatedKORG_MIDI_Synthesizer::ProgramChange(uint32 trackNumber, uint8 programNumber)
{
}

//Private methods
void SoftwareSimulatedKORG_MIDI_Synthesizer::LoadSound(AccompanimentTrackNumber accompanimentTrackNumber, const SoundBankNumber &soundBankNumber, uint8 pos)
{
    const auto& sound = this->playBackSet.GetSound(soundBankNumber, pos);
    ASSERT_EQUALS(true, sound.drumKitData.HasValue());

    this->trackInstruments[(uint8)accompanimentTrackNumber] = new PlayBackDrumKit(sound.drumKitData.Value(), this->playBackSet, *this);
}
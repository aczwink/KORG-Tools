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
#include "PlayBackDrumKit.hpp"

class SoftwareSimulatedKORG_MIDI_Synthesizer : public MIDI::OutputSequencer, public PlayBackFactory
{
public:
    //Constructor
    SoftwareSimulatedKORG_MIDI_Synthesizer(const Set& set);

    //Destructor
    ~SoftwareSimulatedKORG_MIDI_Synthesizer();

    //Methods
    void ControlChange(uint32 trackNumber, uint8 controllerNumber, uint8 controllerValue) override;
    Audio::Source *CreateSource() override;
    Audio::Buffer* LoadSample(uint64 id);
    void NoteOff(uint32 trackNumber, uint8 note, uint8 velocity) override;
    void NoteOn(uint32 trackNumber, uint8 note, uint8 velocity) override;
    void ProgramChange(uint32 trackNumber, uint8 programNumber) override;

private:
    //Variables
    PlayBackSet playBackSet;
    AutoPointer<AudioDevice> audioDevice;
    UniquePointer<Audio::DeviceContext> deviceContext;
    BinaryTreeMap<uint64, Audio::Buffer*> sampleBuffers;
    UniquePointer<PlayBackDrumKit> trackInstruments[8];

    //Methods
    void LoadSound(AccompanimentTrackNumber accompanimentTrackNumber, const SoundBankNumber& soundBankNumber, uint8 pos);
};
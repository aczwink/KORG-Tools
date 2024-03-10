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
#include "StyleMIDILoader.hpp"

//Public methods
MIDI::Program StyleMIDILoader::LoadVariation(uint8 variation, uint8 chordVariation, const Style::StyleData& data) const
{
    MIDI::Program program(384); //TODO: check for correct value

    StyleView styleView(data);
    const IChordVariationView& chordVariationView = styleView.GetVariation(variation).GetChordVariation(chordVariation);

    this->LoadTrackEvents(AccompanimentTrackNumber::Drum, chordVariationView, program);
    this->LoadTrackEvents(AccompanimentTrackNumber::Percussion, chordVariationView, program);
    //TODO: load all

    return program;
}

//Private methods
void StyleMIDILoader::LoadTrackEvents(AccompanimentTrackNumber accompanimentTrackNumber, const IChordVariationView& chordVariationView, MIDI::Program& program) const
{
    uint64 t = 0;
    for(const auto& event : chordVariationView.GetTrack(accompanimentTrackNumber).MIDI_Events().events)
    {
        switch(event.type)
        {
            case Style::KORG_MIDI_EventType::NoteOff:
                program.AddChannelMessage(MIDI::ChannelMessageType::NoteOff, (uint8)accompanimentTrackNumber, t, event.value1, event.value2);
                break;
            case Style::KORG_MIDI_EventType::NoteOn:
                program.AddChannelMessage(MIDI::ChannelMessageType::NoteOn, (uint8)accompanimentTrackNumber, t, event.value1, event.value2);
                break;
            case Style::KORG_MIDI_EventType::ControlChange:
                //ignore for now
                break;
            case Style::KORG_MIDI_EventType::Aftertouch:
                //ignore for now
                break;
            case Style::KORG_MIDI_EventType::Bend:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
                break;
            case Style::KORG_MIDI_EventType::MetaEvent:
            {
                DynamicByteBuffer dynamicByteBuffer;
                dynamicByteBuffer.Resize(event.metaEvent.dataLength);
                dynamicByteBuffer.CopyFrom(event.metaEvent.data, 0, dynamicByteBuffer.Size());
                program.AddMetaEvent((MIDI::MetaEventType)event.metaEvent.type, t, dynamicByteBuffer);
            }
            break;
            case Style::KORG_MIDI_EventType::RXnoiseOff:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
                break;
            case Style::KORG_MIDI_EventType::RXnoiseOn:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
                break;
            case Style::KORG_MIDI_EventType::DeltaTime:
                t += event.value1;
                break;
        }
    }
}

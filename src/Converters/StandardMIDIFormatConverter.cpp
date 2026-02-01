/*
 * Copyright (c) 2022-2026 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg/Converters/StandardMIDIFormatConverter.hpp>
#include <libkorg/Style/KORG_MIDI.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
MIDI::Program StandardMIDIFormatConverter::LoadVariation(uint8 variation, uint8 chordVariation, const Style::StyleData& data)
{
    const uint16 ticksPerQuarter = 384; //as of PA600 manual
    MIDI::Program program(ticksPerQuarter);

	StyleView styleView(data);
	const IStyleElementView& variationView = styleView.GetVariation(variation);
	const IChordVariationView& chordVariationView = variationView.GetChordVariation(chordVariation);
	uint8 bestFPS = this->FindMostPreciseFrameRate(chordVariationView);

    this->AddTempoMessage(this->beatsPerMinute, program);
    this->AddTimeSignatureMessage(program, bestFPS);

    for(const auto trackNumber : AccompanimentTrackNumbers)
    {
        this->LoadTrackEvents(trackNumber, variationView, chordVariation, program, bestFPS);
    }

    program.AddMetaEvent(MIDI::MetaEventType::EndOfTrack, this->endTime, {});

    return program;
}

//Private methods
void StandardMIDIFormatConverter::AddMetaEvent(const Style::KORG_MIDI_Event& event, uint64 scaledTime, MIDI::Program& program)
{
	switch(event.metaEvent.type)
	{
		case Style::KORG_MIDI_MetaEventType::EndOfTrack:
			this->endTime = Math::Max(this->endTime, scaledTime);
			return;
	}

	DynamicByteBuffer dynamicByteBuffer;
	dynamicByteBuffer.Resize(event.metaEvent.dataLength);
	dynamicByteBuffer.CopyFrom(event.metaEvent.data, 0, dynamicByteBuffer.Size());

	program.AddMetaEvent((MIDI::MetaEventType)event.metaEvent.type, scaledTime, dynamicByteBuffer);
}

void StandardMIDIFormatConverter::LoadTrackEvents(AccompanimentTrackNumber accompanimentTrackNumber, const IStyleElementView& styleElementView, uint8 chordVariation, MIDI::Program& program, uint8 targetFPS)
{
	const auto& trackMetaData = styleElementView.GetStyleTrackMetaData(accompanimentTrackNumber);
	uint8 gmChannel = this->MapTrackNumberToGeneralMIDIChannel(accompanimentTrackNumber);

	//from PA600 manual p.148 Exporting a Style as an SMF separated by Markers: standard midi will have the following at the beginning of each chord variation:
	program.AddControlChangeMessage(gmChannel, 0, MIDI::ControlChangeMessageType::Expression, trackMetaData.expression);
	program.AddControlChangeMessage(gmChannel, 0, MIDI::ControlChangeMessageType::BankSelect, trackMetaData.soundProgramChangeSeq.BankSelectMSB());
	program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 32, trackMetaData.soundProgramChangeSeq.BankSelectLSB());
	program.AddChannelMessage(MIDI::ChannelMessageType::ProgramChange, gmChannel, 0, trackMetaData.soundProgramChangeSeq.ProgramChange(), 0);

	if(this->addNonStandardControlChangeMessages)
	{
		//TODO: need tests for correct values. What is this?!
		program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 114, 21); //something non-standard :S
		program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 115, 108); //something non-standard :S
		program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 116, 0); //something non-standard :S
		program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 117, 0); //something non-standard :S
		program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 118, 0); //something non-standard :S
		program.AddChannelMessage(MIDI::ChannelMessageType::ControlChange, gmChannel, 0, 119, 1); //something non-standard :S
	}

	const IChordVariationView& chordVariationView = styleElementView.GetChordVariation(chordVariation);
    const auto& track = chordVariationView.GetTrack(accompanimentTrackNumber).MIDI_Events();
    Math::Rational<uint64> timeScale(targetFPS, track.timeScale);

	uint64 t = 0;
    for(const auto& event : track.events)
    {
        auto scaledTime = (t * timeScale).DivideAndRoundDown();
        switch(event.type)
        {
			case Style::KORG_MIDI_EventType::RXnoiseOff: //RX noises are mapped to standard notes
            case Style::KORG_MIDI_EventType::NoteOff:
                program.AddChannelMessage(MIDI::ChannelMessageType::NoteOff, gmChannel, scaledTime, event.value1, event.value2);
                break;
			case Style::KORG_MIDI_EventType::RXnoiseOn: //RX noises are mapped to standard notes
            case Style::KORG_MIDI_EventType::NoteOn:
                program.AddChannelMessage(MIDI::ChannelMessageType::NoteOn, gmChannel, scaledTime, event.value1, event.value2);
                break;
            case Style::KORG_MIDI_EventType::ControlChange:
            	program.AddControlChangeMessage(gmChannel, scaledTime, static_cast<MIDI::ControlChangeMessageType>(event.value1), event.value2);
                break;
            case Style::KORG_MIDI_EventType::Aftertouch:
            	program.AddChannelMessage(MIDI::ChannelMessageType::ChannelPressure, gmChannel, scaledTime, event.value1);
                break;
            case Style::KORG_MIDI_EventType::Bend:
			{
				int32 pitch = event.value1 + 8192;
				program.AddChannelMessage(MIDI::ChannelMessageType::Pitch, gmChannel, scaledTime, static_cast<uint16>(pitch));
			}
			break;
            case Style::KORG_MIDI_EventType::MetaEvent:
            	this->AddMetaEvent(event, scaledTime, program);
            break;
            case Style::KORG_MIDI_EventType::DeltaTime:
                t += event.value1;
                break;
        }
    }
}

void StandardMIDIFormatConverter::AddTempoMessage(uint16 bpm, MIDI::Program &program) const
{
	uint32 microsecondsPerQuarter = static_cast<uint32>((60_u64 * 1000 * 1000) / bpm);

	DynamicByteBuffer payload;
	payload.Resize(3);
	payload[0] = static_cast<uint8>(microsecondsPerQuarter >> 16);
	payload[1] = static_cast<uint8>((microsecondsPerQuarter >> 8) & 0xFF);
	payload[2] = static_cast<uint8>(microsecondsPerQuarter & 0xFF);
	program.AddMetaEvent(MIDI::MetaEventType::SetTempo, 0, payload);
}

void StandardMIDIFormatConverter::AddTimeSignatureMessage(MIDI::Program &program, uint8 midiClocksPerMetronomeClick) const
{
	DynamicByteBuffer payload;
	payload.Resize(4);

	payload[0] = this->timeSignature.numerator;

	switch(this->timeSignature.denominator)
	{
		case 4:
			payload[1] = 2;
			break;
		case 8:
			payload[1] = 3;
			break;
		case 16:
			payload[1] = 4;
			break;
	}
	payload[2] = midiClocksPerMetronomeClick;
	payload[3] = 8; //8x 1/32 note per quarter

	program.AddMetaEvent(MIDI::MetaEventType::TimeSignature, 0, payload);
}

uint8 StandardMIDIFormatConverter::FindMostPreciseFrameRate(const IChordVariationView &chordVariationView) const
{
	uint8 maxFPS = 0;
	for(const auto trackNumber : AccompanimentTrackNumbers)
	{
		uint8 fps = chordVariationView.GetTrack(trackNumber).MIDI_Events().timeScale;

		maxFPS = Math::Max(fps, maxFPS);
	}

	return maxFPS;
}

uint8 StandardMIDIFormatConverter::MapTrackNumberToGeneralMIDIChannel(AccompanimentTrackNumber accompanimentTrackNumber) const
{
    switch(accompanimentTrackNumber)
    {
		case AccompanimentTrackNumber::Bass:
			return 8;
        case AccompanimentTrackNumber::Drum:
            return 9;
		case AccompanimentTrackNumber::Percussion:
			return 10;
		case AccompanimentTrackNumber::Accompaniment1:
			return 11;
		case AccompanimentTrackNumber::Accompaniment2:
			return 12;
		case AccompanimentTrackNumber::Accompaniment3:
			return 13;
		case AccompanimentTrackNumber::Accompaniment4:
			return 14;
		case AccompanimentTrackNumber::Accompaniment5:
			return 15;
    }
}
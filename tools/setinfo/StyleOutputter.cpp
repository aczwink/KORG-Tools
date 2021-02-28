/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "StyleOutputter.hpp"
//Local
#include "PerformanceOutputter.hpp"

//Public methods
void StyleOutputter::Output(const FullStyle &fullStyle)
{
	this->Output(fullStyle.Style());

	PerformanceOutputter performanceOutputter(this->formattedOutputter);
	//performanceOutputter.Output(fullStyle.STS()); //TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

//Private methods
void StyleOutputter::Output(const Style &style)
{
	Section section(u8"Style Data", this->formattedOutputter);
	this->Output(style.Data());
}

void StyleOutputter::Output(const StyleData &styleData)
{
	{
		Section chunkSection(u8"0x1000308", this->formattedOutputter);

		auto& data = styleData._0x1000308_chunk;
		this->formattedOutputter.OutputProperty(u8"name", data.name);
		this->formattedOutputter.OutputProperty(u8"unknown1", data.unknown1);
		this->formattedOutputter.OutputProperty(u8"unknown2", data.unknown2);
		this->formattedOutputter.OutputProperty(u8"unknown3", data.unknown3);
		this->formattedOutputter.OutputProperty(u8"unknown4", data.unknown4);
		this->formattedOutputter.OutputProperty(u8"unknown5", data.unknown5);
		this->formattedOutputter.OutputProperty(u8"unknown6", data.unknown6);
		this->formattedOutputter.OutputProperty(u8"unknown7", data.unknown7);
		this->formattedOutputter.OutputProperty(u8"unknown8", data.unknown8);
		this->formattedOutputter.OutputProperty(u8"unknown9", data.unknown9);
		this->formattedOutputter.OutputProperty(u8"unknown10", data.unknown10);
		this->formattedOutputter.OutputProperty(u8"unknown11", data.unknown11);
		this->formattedOutputter.OutputProperty(u8"unknown12", data.unknown12);
		this->formattedOutputter.OutputProperty(u8"unknown13", data.unknown13);
		this->formattedOutputter.OutputProperty(u8"unknown14", data.unknown14);
	}

	{
		Section chunkSection(u8"0x1000008", this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"highest", styleData._0x1000008_chunk.highest);
		for(uint32 i = 0; i < styleData._0x1000008_chunk.values.GetNumberOfElements(); i++)
			this->formattedOutputter.OutputProperty(String::Number(i), styleData._0x1000008_chunk.values[i]);
	}

	{
		Section chunkSection(u8"MIDI tracks", this->formattedOutputter);
		for(uint32 i = 0; i < styleData.midiTracks.GetNumberOfElements(); i++)
		{
			const MIDI_Track& midiTrack = styleData.midiTracks[i];

			Section trackSection(u8"MIDI track #" + String::Number(i), this->formattedOutputter);
			this->Output(midiTrack);
		}
	}

	Section styleElements(u8"Style elements", this->formattedOutputter);
	for(uint8 i = 0; i < 4; i++)
	{
		Section variationSection(u8"Variation " + String::Number(i+1), this->formattedOutputter);
		this->Output(styleData.variation[i]);
	}

	for(uint8 i = 0; i < 3; i++)
	{
		Section introSection(u8"Intro " + String::Number(i+1), this->formattedOutputter);
		this->Output(styleData.styleElements[i]);
	}

	for(uint8 i = 0; i < 4; i++)
	{
		Section introSection(u8"Fill " + String::Number(i+1), this->formattedOutputter);
		this->Output(styleData.styleElements[i+3]);
	}

	{
		Section introSection(u8"Break", this->formattedOutputter);
		this->Output(styleData.styleElements[7]);
	}

	for(uint8 i = 0; i < 3; i++)
	{
		Section introSection(u8"Ending " + String::Number(i+1), this->formattedOutputter);
		this->Output(styleData.styleElements[i+8]);
	}
}

void StyleOutputter::Output(const MIDI_Track& midiTrack)
{
	this->formattedOutputter.OutputProperty(u8"chunkId", String::HexNumber(midiTrack.chunkType));
	switch(midiTrack.chunkType)
	{
		case MIDI_Track::CHUNK_0x2000008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x2000008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x2000008_data.unknown2);
			break;
		case MIDI_Track::CHUNK_0x3000008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x3000008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x3000008_data.unknown2);
			this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack._0x3000008_data.unknown3);
			this->formattedOutputter.OutputProperty(u8"unknown4", midiTrack._0x3000008_data.unknown4);
			this->formattedOutputter.OutputProperty(u8"unknown5", midiTrack._0x3000008_data.unknown5);
			break;
		case MIDI_Track::CHUNK_0x4000008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x4000008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x4000008_data.unknown2);
			this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack._0x4000008_data.unknown3);
			this->formattedOutputter.OutputProperty(u8"unknown4", midiTrack._0x4000008_data.unknown4);
			break;
		case MIDI_Track::CHUNK_0x5010008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x5010008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x5010008_data.unknown2);
			this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack._0x5010008_data.unknown3);
			this->formattedOutputter.OutputProperty(u8"unknown4", midiTrack._0x5010008_data.unknown4);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}

	for(uint32 i = 0; i < midiTrack.events.GetNumberOfElements(); i++)
	{
		this->Output(i, midiTrack.events[i]);
	}
}

void StyleOutputter::Output(uint32 index, const KORG_MIDI_Event &event)
{
	Section eventSection(u8"MIDI event #" + String::Number(index), this->formattedOutputter);

	switch(event.type)
	{
		case KORG_MIDI_EventType::NoteOff:
			this->formattedOutputter.OutputProperty(u8"type", u8"note off");
			this->formattedOutputter.OutputProperty(u8"pitch", PitchToString(Pitch(event.value1)));
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			break;
		case KORG_MIDI_EventType::NoteOn:
			this->formattedOutputter.OutputProperty(u8"type", u8"note on");
			this->formattedOutputter.OutputProperty(u8"pitch", PitchToString(Pitch(event.value1)));
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			break;
		case KORG_MIDI_EventType::ControlChange:
			this->formattedOutputter.OutputProperty(u8"type", u8"control change");
			this->formattedOutputter.OutputProperty(u8"number", event.value1);
			this->formattedOutputter.OutputProperty(u8"value", event.value2);
			break;
		case KORG_MIDI_EventType::Aftertouch:
			this->formattedOutputter.OutputProperty(u8"type", u8"aftertouch");
			this->formattedOutputter.OutputProperty(u8"velocity", event.value1);
			break;
		case KORG_MIDI_EventType::Bend:
			this->formattedOutputter.OutputProperty(u8"type", u8"bend");
			this->formattedOutputter.OutputProperty(u8"velocity", (int16)event.value1);
			break;
		case KORG_MIDI_EventType::RXnoiseOff:
			this->formattedOutputter.OutputProperty(u8"type", u8"rxnoise off");
			this->formattedOutputter.OutputProperty(u8"pitch", PitchToString(Pitch(event.value1)));
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			break;
		case KORG_MIDI_EventType::RXnoiseOn:
			this->formattedOutputter.OutputProperty(u8"type", u8"rxnoise on");
			this->formattedOutputter.OutputProperty(u8"pitch", PitchToString(Pitch(event.value1)));
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			break;
		case KORG_MIDI_EventType::EndOfTrack:
			this->formattedOutputter.OutputProperty(u8"type", u8"end of track");
			break;
		case KORG_MIDI_EventType::Unknown:
			this->formattedOutputter.OutputProperty(u8"type", u8"unknown");
			this->formattedOutputter.OutputProperty(u8"nBytes", event.value1);
			break;
		case KORG_MIDI_EventType::DeltaTime:
			this->formattedOutputter.OutputProperty(u8"type", u8"delta time");
			this->formattedOutputter.OutputProperty(u8"velocity", event.value1);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

void StyleOutputter::Output(const VariationStyleElementData& data)
{
	this->OutputGeneralStyleElementData(data);

	for(uint32 i = 0; i < 6; i++)
	{
		Section cvSection(u8"Chord variation " + String::Number(i+1), this->formattedOutputter);
		this->Output(data.cv[i]);
	}
}

void StyleOutputter::Output(const StyleElementData &data)
{
	this->OutputGeneralStyleElementData(data);

	for(uint32 i = 0; i < 2; i++)
	{
		Section cvSection(u8"Chord variation " + String::Number(i+1), this->formattedOutputter);
		this->Output(data.cv[i]);
	}
}

void StyleOutputter::Output(const ChordVariationData &data)
{
	this->Output(data.midiTrack);

	for(uint32 i = 0; i < data.unknown.GetNumberOfElements(); i++)
	{
		this->formattedOutputter.OutputProperty(u8"unknown[" + String::Number(i) + u8"]", data.unknown[i]);
	}
}

void StyleOutputter::OutputGeneralStyleElementData(const GeneralStyleElementData &data)
{
	{
		Section chunkSection(u8"unknown chord table", this->formattedOutputter);

		this->formattedOutputter.OutputUnknownChunk(data.unknownChordTable);
	}

	for(uint32 i = 0; i < 8; i++)
	{
		Section trackSection(AccompanimentTrackNumberToAbbreviatedString(static_cast<AccompanimentTrackNumber>(i)), this->formattedOutputter);
		this->formattedOutputter.OutputProperty(u8"expression", data.styleTrackData[i].expression);
		this->formattedOutputter.OutputProperty(u8"soundProgramChangeSeq", data.styleTrackData[i].soundProgramChangeSeq->ToString());
		this->formattedOutputter.OutputProperty(u8"keyboardRangeBottom", data.styleTrackData[i].keyboardRangeBottom);
		this->formattedOutputter.OutputProperty(u8"keyboardRangeTop", data.styleTrackData[i].keyboardRangeTop);
	}
}
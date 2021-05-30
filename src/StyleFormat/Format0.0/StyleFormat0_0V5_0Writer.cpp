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
#include "StyleFormat0_0V5_0Writer.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Style;
using namespace StdXX;

//Public methods
void StyleFormat0_0V5_0Writer::Write(const StyleData &styleData)
{
	this->BeginChunk(1, 5, 0, 0);

	this->Write0x1000308Chunk(styleData);

	this->BeginChunk(2, 0, 0, 0);
	this->WriteMIDITrackMapping(styleData);
	for(const MIDI_Track& midiTrack : styleData.midiTracks)
		this->WriteMIDITrack(midiTrack);
	this->EndChunk();

	for(const auto & variation : styleData.variation)
		this->WriteStyleElement(variation);
	for(const auto & styleElement : styleData.styleElements)
		this->WriteStyleElement(styleElement);

	this->EndChunk();
}

//Private methods
void StyleFormat0_0V5_0Writer::WriteMIDITrackMapping(const StyleData &styleData)
{
	auto& data = styleData.oneBasedMIDITrackMappingIndices;

	this->BeginChunk(1, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteUInt16(styleData.midiTracks.GetNumberOfElements());
	this->dataWriter.WriteUInt16(data.GetNumberOfElements());
	for(uint16 value : data)
		this->dataWriter.WriteUInt16(value);

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::Write0x1000308Chunk(const StyleData& styleData)
{
	auto& data = styleData._0x1000308_chunk;
	TextWriter textWriter(this->outputStream, TextCodecType::ASCII);

	this->BeginChunk(1, 0, 3, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(data.name.GetLength());
	textWriter.WriteString(data.name);

	this->dataWriter.WriteByte(data.unknown1);
	this->dataWriter.WriteByte(data.unknown2);
	this->dataWriter.WriteByte(data.unknown3);
	this->dataWriter.WriteUInt16(data.enabledStyleElements);
	this->dataWriter.WriteByte(data.unknown6);
	this->dataWriter.WriteByte(data.unknown7);
	this->dataWriter.WriteByte(data.unknown8);
	this->dataWriter.WriteByte(data.unknown9);
	this->dataWriter.WriteByte(data.unknown10);
	this->dataWriter.WriteByte(data.unknown11);
	this->dataWriter.WriteByte(data.unknown12);
	this->dataWriter.WriteByte(data.unknown13);
	this->dataWriter.WriteByte(data.unknown14);

	this->dataWriter.WriteByte(0);
	this->dataWriter.WriteByte(0);
	this->dataWriter.WriteByte(0x7F);
	this->dataWriter.WriteByte(0xFF);

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::Write0x2000308Chunk(const GeneralStyleElementData &styleElementData)
{
	uint8 versionMinor;
	switch(styleElementData.unknown.Size())
	{
		case 40:
			versionMinor = 3;
			break;
		case 48:
			versionMinor = 4;
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
	this->BeginChunk(2, 0, versionMinor, ChunkHeaderFlags::Leaf);

	for(const StyleTrackData& styleTrackData : styleElementData.styleTrackData)
	{
		this->dataWriter.WriteByte(styleTrackData.expression);
		styleTrackData.soundProgramChangeSeq.Write(this->dataWriter);
		this->dataWriter.WriteByte(styleTrackData.keyboardRangeBottom.Encode());
		this->dataWriter.WriteByte(styleTrackData.keyboardRangeTop.Encode());
	}
	styleElementData.unknown.CreateInputStream()->FlushTo(this->outputStream);

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::WriteChordTable(const ChordTable& chordTable)
{
	uint8 versionMinor = 1;
	switch (chordTable.unknown.Size())
	{
		case 15:
			versionMinor = 2;
			break;
		case 16:
			versionMinor = 3;
			break;
	}

	this->BeginChunk(1, 1, versionMinor, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(chordTable.unknown1);
	this->dataWriter.WriteByte(chordTable.unknown2);

	this->dataWriter.WriteByte(24);

	this->dataWriter.WriteByte(chordTable.majorCVIndex);
	this->dataWriter.WriteByte(chordTable.sixCVIndex);
	this->dataWriter.WriteByte(chordTable.M7CVIndex);
	this->dataWriter.WriteByte(chordTable.M7b5CVIndex);
	this->dataWriter.WriteByte(chordTable.susCVIndex);
	this->dataWriter.WriteByte(chordTable.sus2CVIndex);
	this->dataWriter.WriteByte(chordTable.M7susCVIndex);
	this->dataWriter.WriteByte(chordTable.mCVIndex);
	this->dataWriter.WriteByte(chordTable.m6CVIndex);
	this->dataWriter.WriteByte(chordTable.m7CVIndex);
	this->dataWriter.WriteByte(chordTable.m7b5CVIndex);
	this->dataWriter.WriteByte(chordTable.mM7CVIndex);
	this->dataWriter.WriteByte(chordTable.sevenCVIndex);
	this->dataWriter.WriteByte(chordTable.seven_b5CVIndex);
	this->dataWriter.WriteByte(chordTable.seven_susCVIndex);
	this->dataWriter.WriteByte(chordTable.dimCVIndex);
	this->dataWriter.WriteByte(chordTable.dimM7CVIndex);
	this->dataWriter.WriteByte(chordTable.sharp5CVIndex);
	this->dataWriter.WriteByte(chordTable.seven_sharp5CVIndex);
	this->dataWriter.WriteByte(chordTable.M7sharp5CVIndex);
	this->dataWriter.WriteByte(chordTable.onePlusFiveCVIndex);
	this->dataWriter.WriteByte(chordTable.onePlusEightCVIndex);
	this->dataWriter.WriteByte(chordTable.b5CVIndex);
	this->dataWriter.WriteByte(chordTable.dim7CVIndex);
	this->dataWriter.WriteByte(chordTable.unknown3);

	chordTable.unknown.CreateInputStream()->FlushTo(this->dataWriter.Stream());

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::WriteChordVariationData(const ChordVariationData &cv)
{
	this->BeginChunk(3, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(cv.masterMidiTrack.unknown1);
	this->dataWriter.WriteByte(cv.masterMidiTrack.unknown2);
	this->dataWriter.WriteByte(cv.masterMidiTrack.unknown3);
	this->dataWriter.WriteByte(0);

	this->BeginSizeBracket16();
	for(const auto& event : cv.masterMidiTrack.events)
		this->WriteMIDIEvent(event);
	this->EndSizeBracket16();

	this->dataWriter.WriteByte(cv.trackMapping.GetNumberOfElements());
	for(const auto& mapping : cv.trackMapping)
	{
		this->dataWriter.WriteByte(static_cast<byte>(mapping.type));
		this->dataWriter.WriteByte(static_cast<byte>(mapping.trackNumber));
	}

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::WriteMIDIEvent(const KORG_MIDI_Event &event)
{
	switch(event.type)
	{
		case KORG_MIDI_EventType::NoteOff:
			this->dataWriter.WriteByte(0);
			this->dataWriter.WriteByte(event.value1);
			this->dataWriter.WriteByte(event.value2 | 0x80);
			break;
		case KORG_MIDI_EventType::NoteOn:
			this->dataWriter.WriteByte(1);
			this->dataWriter.WriteByte(event.value1);
			this->dataWriter.WriteByte(event.value2 | 0x80);
			break;
		case KORG_MIDI_EventType::ControlChange:
			this->dataWriter.WriteByte(3);
			this->dataWriter.WriteByte(event.value1);
			this->dataWriter.WriteByte(event.value2 | 0x80);
			break;
		case KORG_MIDI_EventType::Aftertouch:
			this->dataWriter.WriteByte(5);
			this->dataWriter.WriteByte(event.value1 | 0x80);
			break;
		case KORG_MIDI_EventType::Bend:
		{
			this->dataWriter.WriteByte(6);
			uint16 value1 = int16(event.value1) + 8192;
			this->dataWriter.WriteByte(value1 & 0x7F);
			this->dataWriter.WriteByte((value1 >> 7) | 0x80);
		}
		break;
		case KORG_MIDI_EventType::RXnoiseOff:
			this->dataWriter.WriteByte(12);
			this->dataWriter.WriteByte(event.value1);
			this->dataWriter.WriteByte(event.value2 | 0x80);
			break;
		case KORG_MIDI_EventType::RXnoiseOn:
			this->dataWriter.WriteByte(13);
			this->dataWriter.WriteByte(event.value1);
			this->dataWriter.WriteByte(event.value2 | 0x80);
			break;
		case KORG_MIDI_EventType::EndOfTrack:
			this->dataWriter.WriteByte(9);
			this->dataWriter.WriteByte(0x2F);
			this->dataWriter.WriteUInt16(0);
			break;
		case KORG_MIDI_EventType::Unknown:
			this->dataWriter.WriteByte(9);
			this->dataWriter.WriteByte(0x7E);
			this->dataWriter.WriteUInt16(event.value1);
			this->dataWriter.WriteBytes(&event.additional9Bytes[0], event.value1);
			break;
		case KORG_MIDI_EventType::DeltaTime:
		{
			uint16 v = event.value1;
			DynamicArray<uint8> bytes;
			while(v)
			{
				bytes.Push((v & 0x7F) | 0x80);
				v >>= 7;
			}
			while(!bytes.IsEmpty())
				this->dataWriter.WriteByte(bytes.Pop());
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

void StyleFormat0_0V5_0Writer::WriteMIDITrack(const MIDI_Track &midiTrack)
{
	switch (midiTrack.chunkType)
	{
		case MIDI_Track::CHUNK_0x2000008:
			this->BeginChunk(2, 0, 0, ChunkHeaderFlags::Leaf);
			this->dataWriter.WriteUInt16(0);
			this->dataWriter.WriteByte(midiTrack._0x2000008_data.unknown1);
			this->dataWriter.WriteByte(midiTrack._0x2000008_data.unknown2);
			this->dataWriter.WriteUInt16(0);
			break;
		case MIDI_Track::CHUNK_0x3000008:
			this->BeginChunk(3, 0, 0, ChunkHeaderFlags::Leaf);
			this->dataWriter.WriteUInt16(0);
			this->dataWriter.WriteByte(midiTrack._0x3000008_data.unknown1);
			this->dataWriter.WriteByte(midiTrack._0x3000008_data.unknown2);
			this->dataWriter.WriteByte(midiTrack._0x3000008_data.unknown3);
			this->dataWriter.WriteByte(0);
			break;
		case MIDI_Track::CHUNK_0x4000008:
			this->BeginChunk(4, 0, 0, ChunkHeaderFlags::Leaf);
			this->dataWriter.WriteUInt16(0);
			this->dataWriter.WriteByte(midiTrack._0x4000008_data.unknown1);
			this->dataWriter.WriteByte(midiTrack._0x4000008_data.unknown2);
			this->dataWriter.WriteByte(0);
			this->dataWriter.WriteByte(0);
			break;
		case MIDI_Track::CHUNK_0x5010008:
			this->BeginChunk(5, 1, 0, ChunkHeaderFlags::Leaf);
			this->dataWriter.WriteUInt16(0);
			this->dataWriter.WriteByte(midiTrack._0x5010008_data.unknown1);
			this->dataWriter.WriteByte(midiTrack._0x5010008_data.unknown2);
			this->dataWriter.WriteByte(0);
			this->dataWriter.WriteByte(0);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}

	this->BeginSizeBracket16();
	for(const auto& event : midiTrack.events)
		this->WriteMIDIEvent(event);
	this->EndSizeBracket16();

	switch(midiTrack.chunkType)
	{
		case MIDI_Track::CHUNK_0x3000008:
			this->dataWriter.WriteByte(midiTrack._0x3000008_data.unknown4);
			this->dataWriter.WriteByte(midiTrack._0x3000008_data.unknown5);
			break;
		case MIDI_Track::CHUNK_0x4000008:
			this->dataWriter.WriteByte(midiTrack._0x4000008_data.unknown3);
			this->dataWriter.WriteByte(midiTrack._0x4000008_data.unknown4);
			break;
		case MIDI_Track::CHUNK_0x5010008:
			this->dataWriter.WriteByte(midiTrack._0x5010008_data.unknown3);
			this->dataWriter.WriteByte(midiTrack._0x5010008_data.unknown4);
			break;
	}

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::WriteStyleElement(const VariationStyleElementData &styleElementData)
{
	this->BeginChunk(3, 0, 0, 0);

	this->WriteChordTable(styleElementData.chordTable);
	this->Write0x2000308Chunk(styleElementData);
	for(const ChordVariationData& cv : styleElementData.cv)
		this->WriteChordVariationData(cv);

	this->EndChunk();
}

void StyleFormat0_0V5_0Writer::WriteStyleElement(const StyleElementData &styleElementData)
{
	this->BeginChunk(3, 0, 0, 0);

	this->WriteChordTable(styleElementData.chordTable);
	this->Write0x2000308Chunk(styleElementData);
	for(const ChordVariationData& cv : styleElementData.cv)
		this->WriteChordVariationData(cv);

	this->EndChunk();
}
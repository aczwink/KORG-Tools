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
#include "StyleWriter.hpp"
//Namespaces
using namespace libKORG;

//Public methods
void StyleWriter::Write(const Style &style)
{
	const StyleData &styleData = style.Data();

	this->BeginChunk(1, 5, 0, 0);

	this->Write0x1000308Chunk(styleData);

	this->BeginChunk(2, 0, 0, 0);
	this->Write0x1000008Chunk(styleData);
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
void StyleWriter::Write0x1000008Chunk(const StyleData &styleData)
{
	auto& chunk = styleData._0x1000008_chunk;

	this->BeginChunk(1, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteUInt16(chunk.highest);
	this->dataWriter.WriteUInt16(chunk.values.GetNumberOfElements());
	for(uint16 value : chunk.values)
		this->dataWriter.WriteUInt16(value);

	this->EndChunk();
}

void StyleWriter::Write0x1000308Chunk(const libKORG::StyleData& styleData)
{
	auto& data = styleData._0x1000308_chunk;
	TextWriter textWriter(this->outputStream, TextCodecType::ASCII);

	this->BeginChunk(1, 0, 3, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(data.name.GetLength());
	textWriter.WriteString(data.name);

	this->dataWriter.WriteByte(data.unknown1);
	this->dataWriter.WriteByte(data.unknown2);
	this->dataWriter.WriteByte(data.unknown3);
	this->dataWriter.WriteByte(data.unknown4);
	this->dataWriter.WriteByte(data.unknown5);
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

void StyleWriter::Write0x2000308Chunk(const GeneralStyleElementData &styleElementData)
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
		styleTrackData.soundProgramChangeSeq->Write(this->dataWriter);
		this->dataWriter.WriteByte(styleTrackData.keyboardRangeBottom.Encode());
		this->dataWriter.WriteByte(styleTrackData.keyboardRangeTop.Encode());
	}
	styleElementData.unknown.CreateInputStream()->FlushTo(this->outputStream);

	this->EndChunk();
}

void StyleWriter::WriteChordVariationData(const ChordVariationData &cv)
{
	this->BeginChunk(3, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(cv.midiTrack._0x3000008_data.unknown1);
	this->dataWriter.WriteByte(cv.midiTrack._0x3000008_data.unknown2);
	this->dataWriter.WriteByte(cv.midiTrack._0x3000008_data.unknown3);
	this->dataWriter.WriteByte(0);

	this->BeginSizeBracket16();
	for(const auto& event : cv.midiTrack.events)
		this->WriteMIDIEvent(event);
	this->EndSizeBracket16();

	this->dataWriter.WriteByte(cv.unknown.GetNumberOfElements());
	for(uint16 v : cv.unknown)
		this->dataWriter.WriteUInt16(v);

	this->EndChunk();
}

void StyleWriter::WriteMIDIEvent(const KORG_MIDI_Event &event)
{
	switch (event.type)
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
			this->dataWriter.WriteByte(event.value1 | 0x80);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

void StyleWriter::WriteMIDITrack(const MIDI_Track &midiTrack)
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

void StyleWriter::WriteStyleElement(const VariationStyleElementData &styleElementData)
{
	this->BeginChunk(3, 0, 0, 0);

	this->WriteUnknownChunk(styleElementData.unknownChordTable);
	this->Write0x2000308Chunk(styleElementData);
	for(const ChordVariationData& cv : styleElementData.cv)
		this->WriteChordVariationData(cv);

	this->EndChunk();
}

void StyleWriter::WriteStyleElement(const StyleElementData &styleElementData)
{
	this->BeginChunk(3, 0, 0, 0);

	this->WriteUnknownChunk(styleElementData.unknownChordTable);
	this->Write0x2000308Chunk(styleElementData);
	for(const ChordVariationData& cv : styleElementData.cv)
		this->WriteChordVariationData(cv);

	this->EndChunk();
}
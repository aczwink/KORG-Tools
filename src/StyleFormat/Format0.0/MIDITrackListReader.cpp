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
#include "MIDITrackListReader.hpp"
//Local
#include "KORG_MIDI_EventDecoder.hpp"
//Namespaces
using namespace libKORG::Style;
using namespace StdXX;

//Protected methods
void MIDITrackListReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x01000008:
			this->ReadMIDITrackMapping(dataReader);
			break;
		case 0x02000008:
			this->Read0x2000008Chunk(dataReader);
			break;
		case 0x03000008:
			this->Read0x3000008Chunk(dataReader);
			break;
		case 0x4000008:
			this->Read0x4000008Chunk(dataReader);
			break;
		case 0x05000108:
		case 0x05010008:
			this->Read0x5010008Chunk(dataReader);
			break;
	}
}

//Private methods
void MIDITrackListReader::Read0x2000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::CHUNK_0x2000008;

	midiTrack._0x2000008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x2000008_data.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);
}

void MIDITrackListReader::Read0x3000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::CHUNK_0x3000008;

	midiTrack._0x3000008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x3000008_data.unknown2 = dataReader.ReadByte();
	midiTrack._0x3000008_data.unknown3 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack._0x3000008_data.unknown4 = dataReader.ReadByte();
	midiTrack._0x3000008_data.unknown5 = dataReader.ReadByte();
}

void MIDITrackListReader::Read0x4000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::CHUNK_0x4000008;

	midiTrack._0x4000008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x4000008_data.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack._0x4000008_data.unknown3 = dataReader.ReadByte();
	midiTrack._0x4000008_data.unknown4 = dataReader.ReadByte();
}

void MIDITrackListReader::Read0x5010008Chunk(DataReader &dataReader)
{
	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::CHUNK_0x5010008;

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	midiTrack._0x5010008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x5010008_data.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack._0x5010008_data.unknown3 = dataReader.ReadByte();
	midiTrack._0x5010008_data.unknown4 = dataReader.ReadByte();
}

void MIDITrackListReader::ReadKORG_MIDIEvents(uint16 dataLength, DynamicArray<libKORG::Style::KORG_MIDI_Event> &midiEvents, DataReader &dataReader)
{
	KORG_MIDI_EventDecoder decoder;
	midiEvents = decoder.DecodeEvents(dataLength, dataReader);
}

void MIDITrackListReader::ReadMIDITrackMapping(DataReader &dataReader)
{
	auto& data = this->data.oneBasedMIDITrackMappingIndices;

	dataReader.ReadUInt16(); //number of midi tracks

	uint16 nEntries = dataReader.ReadUInt16();
	data.Resize(nEntries);

	for(uint16 i = 0; i < nEntries; i++)
	{
		uint16 v = dataReader.ReadUInt16();
		data[i] = v;
	}
}
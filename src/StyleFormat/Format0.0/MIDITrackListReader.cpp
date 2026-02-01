/*
 * Copyright (c) 2020-2026 Amir Czwink (amir130@hotmail.de)
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
			this->ReadDrumOrPercTrackChunk(dataReader);
			break;
		case 0x03000008:
			this->ReadBassTrackChunk(dataReader);
			break;
		case 0x4000008:
			this->ReadAccompanymentTrackChunk(dataReader);
			break;
		case 0x05000108:
		case 0x05010008:
			this->ReadGuitarTrackChunk(dataReader);
			break;
	}
}

//Private methods
void MIDITrackListReader::ReadAccompanymentTrackChunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::Accompaniment;

	midiTrack.timeScale = dataReader.ReadByte();
	midiTrack.accompanimentData.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack.accompanimentData.unknown3 = dataReader.ReadByte();
	midiTrack.accompanimentData.unknown4 = dataReader.ReadByte();
}

void MIDITrackListReader::ReadBassTrackChunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::Bass;

	midiTrack.timeScale = dataReader.ReadByte();
	midiTrack.bassData.unknown2 = dataReader.ReadByte();
	midiTrack.bassData.unknown3 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack.bassData.unknown4 = dataReader.ReadByte();
	midiTrack.bassData.unknown5 = dataReader.ReadByte();
}

void MIDITrackListReader::ReadDrumOrPercTrackChunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::DrumOrPerc;

	midiTrack.timeScale = dataReader.ReadByte();
	midiTrack.drumOrPercData.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);
}

void MIDITrackListReader::ReadGuitarTrackChunk(DataReader &dataReader)
{
	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = MIDI_Track::Guitar;

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	midiTrack.timeScale = dataReader.ReadByte();
	midiTrack.guitarData.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack.guitarData.unknown3 = dataReader.ReadByte();
	midiTrack.guitarData.unknown4 = dataReader.ReadByte();
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
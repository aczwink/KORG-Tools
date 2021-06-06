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
#include "StyleFormat0_0V5_0Reader.hpp"
//Local
#include <libkorg/Text.hpp>
//Namespaces
using namespace libKORG;
using namespace libKORG::Style;
using namespace StdXX;

//Constructor
StyleFormat0_0V5_0Reader::StyleFormat0_0V5_0Reader()
{
	this->currentStyleElementNumber = 0;
}

//Public methods
libKORG::StyleObject *StyleFormat0_0V5_0Reader::TakeResult()
{
	return new StyleObject(Move(this->data));
}

//Protected methods
void StyleFormat0_0V5_0Reader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	if(this->parentChunkId == 0x3000000)
	{
		switch(chunkHeader.type)
		{
			case 1:
				this->ReadChordTable(chunkHeader.version, this->GetCurrentStyleElementData().chordTable, dataReader);
				break;
			case 2:
				this->ReadStyleTrackDataChunk(chunkHeader.version, dataReader);
				break;
			case 3:
				ASSERT_EQUALS(0, chunkHeader.version.AsUInt16());
				this->ReadMasterMIDITrack(dataReader);
				break;
		}
	}
	else
	{
		switch(chunkHeader.id)
		{
			case 0x1000008:
				this->ReadMIDITrackMapping(dataReader);
				break;
			case 0x1000308:
				this->Read0x1000308Chunk(dataReader);
				break;
			case 0x2000008:
				this->Read0x2000008Chunk(dataReader);
				break;
			case 0x3000008:
				this->Read0x3000008Chunk(dataReader);
				break;
			case 0x4000008:
				this->Read0x4000008Chunk(dataReader);
				break;
			case 0x5010008:
				this->Read0x5010008Chunk(dataReader);
				break;
		}
	}
}

ChunkReader* StyleFormat0_0V5_0Reader::OnEnteringChunk(const ChunkHeader& chunkHeader)
{
	this->parentChunkId = chunkHeader.id;

	if(chunkHeader.id == 0x3000000)
	{
		this->nextMIDITrackNumber = 0;
	}

	return this;
}

void StyleFormat0_0V5_0Reader::OnLeavingChunk(const ChunkHeader& chunkHeader)
{
	if(chunkHeader.id == 0x3000000)
	{
		this->currentStyleElementNumber++;
	}
}

//Private methods
void StyleFormat0_0V5_0Reader::ReadChordTable(const ChunkVersion &chunkVersion, ChordTable &chordTable, DataReader &dataReader)
{
	switch(chunkVersion.AsUInt16())
	{
		case 0x0101:
			this->ReadChordTableV1_1(chordTable, dataReader);
			break;
		case 0x0102:
		case 0x0103:
			this->ReadChordTableV1_1(chordTable, dataReader);
			dataReader.InputStream().FlushTo(*chordTable.unknown.CreateOutputStream());
			break;
	}
}

void StyleFormat0_0V5_0Reader::ReadChordTableV1_1(ChordTable& chordTable, DataReader &dataReader)
{
	chordTable.unknown1 = dataReader.ReadByte();
	chordTable.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(24, dataReader.ReadByte()); //number of entries in the chord table

	chordTable.majorCVIndex = dataReader.ReadByte();
	chordTable.sixCVIndex = dataReader.ReadByte();
	chordTable.M7CVIndex = dataReader.ReadByte();
	chordTable.M7b5CVIndex = dataReader.ReadByte();
	chordTable.susCVIndex = dataReader.ReadByte();
	chordTable.sus2CVIndex = dataReader.ReadByte();
	chordTable.M7susCVIndex = dataReader.ReadByte();
	chordTable.mCVIndex = dataReader.ReadByte();
	chordTable.m6CVIndex = dataReader.ReadByte();
	chordTable.m7CVIndex = dataReader.ReadByte();
	chordTable.m7b5CVIndex = dataReader.ReadByte();
	chordTable.mM7CVIndex = dataReader.ReadByte();
	chordTable.sevenCVIndex = dataReader.ReadByte();
	chordTable.seven_b5CVIndex = dataReader.ReadByte();
	chordTable.seven_susCVIndex = dataReader.ReadByte();
	chordTable.dimCVIndex = dataReader.ReadByte();
	chordTable.dimM7CVIndex = dataReader.ReadByte();
	chordTable.sharp5CVIndex = dataReader.ReadByte();
	chordTable.seven_sharp5CVIndex = dataReader.ReadByte();
	chordTable.M7sharp5CVIndex = dataReader.ReadByte();
	chordTable.onePlusFiveCVIndex = dataReader.ReadByte();
	chordTable.onePlusEightCVIndex = dataReader.ReadByte();
	chordTable.b5CVIndex = dataReader.ReadByte();
	chordTable.dim7CVIndex = dataReader.ReadByte();
	chordTable.unknown3 = dataReader.ReadByte();
}

void StyleFormat0_0V5_0Reader::ReadMasterMIDITrack(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetCurrentStyleElementTrack();

	midiTrack.unknown1 = dataReader.ReadByte();
	midiTrack.unknown2 = dataReader.ReadByte();
	midiTrack.unknown3 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	auto&cv = this->GetCurrentChordVariationData();

	uint8 nEntries = dataReader.ReadByte();
	cv.trackMapping.Resize(nEntries);
	for (uint8 i = 0; i < nEntries; i++)
	{
		cv.trackMapping[i].type = static_cast<ChordVariationTrackType>(dataReader.ReadByte());
		cv.trackMapping[i].trackNumber = static_cast<AccompanimentTrackNumber>(dataReader.ReadByte());
	}

	this->nextMIDITrackNumber++;
}

void StyleFormat0_0V5_0Reader::ReadMIDITrackMapping(DataReader &dataReader)
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

void StyleFormat0_0V5_0Reader::Read0x1000308Chunk(DataReader &dataReader)
{
	auto& chunk = this->data._0x1000308_chunk;

	uint8 nameLength = dataReader.ReadByte();
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	chunk.name = textReader.ReadString(nameLength);

	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadByte();

	chunk.enabledStyleElements = dataReader.ReadUInt16();

	chunk.unknown6 = dataReader.ReadByte();
	chunk.unknown7 = dataReader.ReadByte();
	chunk.unknown8 = dataReader.ReadByte();

	chunk.unknown9 = dataReader.ReadByte();
	chunk.unknown10 = dataReader.ReadByte();
	chunk.unknown11 = dataReader.ReadByte();
	chunk.unknown12 = dataReader.ReadByte();
	chunk.unknown13 = dataReader.ReadByte();
	chunk.unknown14 = dataReader.ReadByte();
	chunk.unknown15 = dataReader.ReadByte();
	chunk.unknown16 = dataReader.ReadByte();
	chunk.unknown17 = dataReader.ReadByte();
	chunk.unknown18 = dataReader.ReadByte();
}

void StyleFormat0_0V5_0Reader::Read0x2000008Chunk(DataReader &dataReader)
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

void StyleFormat0_0V5_0Reader::Read0x3000008Chunk(DataReader &dataReader)
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

void StyleFormat0_0V5_0Reader::Read0x4000008Chunk(DataReader &dataReader)
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

void StyleFormat0_0V5_0Reader::Read0x5010008Chunk(DataReader &dataReader)
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

void StyleFormat0_0V5_0Reader::ReadKORG_MetaEvent(KORG_MIDI_Event& event, DataReader &dataReader)
{
	event.type = KORG_MIDI_EventType::MetaEvent;

	event.metaEvent.type = static_cast<KORG_MIDI_MetaEventType>(dataReader.ReadByte());
	event.metaEvent.dataLength = this->ReadLength(dataReader);

	switch(event.metaEvent.type)
	{
		case KORG_MIDI_MetaEventType::EndOfTrack:
		case KORG_MIDI_MetaEventType::UnknownMaster:
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	ASSERT((event.metaEvent.dataLength <= sizeof(event.metaEvent.data)), String::Number(event.metaEvent.dataLength));
	dataReader.ReadBytes(event.metaEvent.data, event.metaEvent.dataLength);
}

void StyleFormat0_0V5_0Reader::ReadKORG_MIDIEvents(uint16 dataLength, DynamicArray<KORG_MIDI_Event>& midiEvents, DataReader &dataReader)
{
	midiEvents.EnsureCapacity(dataLength * 2 / 3); //Events are either 1 or 2 bytes, with the exception being 0x40 and end of track marker

	bool foundEndOfMarker = false;
	while(!foundEndOfMarker && dataLength)
	{
		uint8 eventType = dataReader.ReadByte();
		dataLength--;

		if(eventType & 0x80)
		{
			uint16 totalLength = 0;
			while(eventType & 0x80)
			{
				uint8 length = eventType & ~0x80;
				totalLength = (totalLength << 7) | length;

				eventType = dataReader.ReadByte();
				dataLength--;
			}

			midiEvents.Push({KORG_MIDI_EventType::DeltaTime, totalLength});
		}

		Optional<uint8> unknownAdditional;
		if(eventType & 0x40)
		{
			eventType &= ~0x40;
			ASSERT((eventType == 0)
				   || (eventType == 1)
				   || (eventType == 3)
				   || (eventType == 6), String::HexNumber(eventType));
			dataLength--;

			unknownAdditional = dataReader.ReadByte(); //TODO: no idea what that is
		}

		switch(eventType)
		{
			case 0:
			case 1:
			case 3:
			case 12:
			case 13:
			{
				uint8 value1 = dataReader.ReadByte();

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				dataLength -= 2;

				midiEvents.Push({(KORG_MIDI_EventType)eventType, value1, value2, unknownAdditional});
			}
			break;
			case 5:
			{
				uint8 value1 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value1 & 0x80);
				value1 &= ~0x80;

				dataLength--;

				midiEvents.Push({KORG_MIDI_EventType::Aftertouch, value1});
			}
			break;
			case 6:
			{
				uint8 b1 = dataReader.ReadByte();
				ASSERT_EQUALS(0, b1 & 0x80);

				uint8 b2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, b2 & 0x80);
				b2 &= ~0x80;
				dataLength -= 2;

				uint16 combined = (b2 << 7) | b1;
				int16 value1 = -8192 + combined;
				midiEvents.Push({KORG_MIDI_EventType::Bend, static_cast<uint16>(value1), 0, unknownAdditional});
			}
			break;
			case 9:
			{
				uint32 index = midiEvents.Push({});
				auto& event = midiEvents[index];
				this->ReadKORG_MetaEvent(event, dataReader);
				dataLength -= 3; //type and size
				dataLength -= event.metaEvent.dataLength;
				foundEndOfMarker = event.metaEvent.type == KORG_MIDI_MetaEventType::EndOfTrack;
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
}

StyleTrackData StyleFormat0_0V5_0Reader::ReadStyleTrackData(DataReader &dataReader)
{
	StyleTrackData styleTrackData;

	styleTrackData.expression = dataReader.ReadByte();

	uint8 msb = dataReader.ReadByte();
	uint8 lsb = dataReader.ReadByte();
	uint8 programChange = dataReader.ReadByte();
	styleTrackData.soundProgramChangeSeq = ProgramChangeSequence(msb, lsb, programChange);

	styleTrackData.keyboardRangeBottom = static_cast<Pitch>(dataReader.ReadByte());
	styleTrackData.keyboardRangeTop = static_cast<Pitch>(dataReader.ReadByte());

	return styleTrackData;
}

void StyleFormat0_0V5_0Reader::ReadStyleTrackDataChunk(const ChunkVersion &chunkVersion, DataReader &dataReader)
{
	switch (chunkVersion.AsUInt16())
	{
		case 0x0003:
		case 0x0004:
			this->ReadStyleTrackDataChunkV0_3(dataReader);
			break;
	}
}

void StyleFormat0_0V5_0Reader::ReadStyleTrackDataChunkV0_3(DataReader &dataReader)
{
	auto& styleElementData = this->GetCurrentStyleElementData();

	styleElementData.styleTrackData[0] = this->ReadStyleTrackData(dataReader); //drums
	styleElementData.styleTrackData[1] = this->ReadStyleTrackData(dataReader); //percussion
	styleElementData.styleTrackData[2] = this->ReadStyleTrackData(dataReader); //bass
	styleElementData.styleTrackData[3] = this->ReadStyleTrackData(dataReader); //acc1
	styleElementData.styleTrackData[4] = this->ReadStyleTrackData(dataReader); //acc2
	styleElementData.styleTrackData[5] = this->ReadStyleTrackData(dataReader); //acc3
	styleElementData.styleTrackData[6] = this->ReadStyleTrackData(dataReader); //acc4
	styleElementData.styleTrackData[7] = this->ReadStyleTrackData(dataReader); //acc5

	auto output = styleElementData.unknown.CreateOutputStream();
	dataReader.InputStream().FlushTo(*output);
}
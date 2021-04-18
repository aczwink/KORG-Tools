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
#include "StyleReader.hpp"
//Local
#include <libkorg/Text.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Constructor
StyleReader::StyleReader()
{
	this->currentStyleElementNumber = 0;
}

//Public methods
libKORG::Style *StyleReader::TakeResult()
{
	return new Style(Move(this->data));
}

//Protected methods
String StyleReader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/STYLE/";
}

void StyleReader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x1000008:
			this->Read0x1000008Chunk(dataReader);
			break;
		case 0x1000308:
			this->Read0x1000308Chunk(dataReader);
			break;
		case 0x2000008:
			this->Read0x2000008Chunk(dataReader);
			break;
		case 0x20000FD:
			this->Read0x20000FDChunk(dataReader);
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

	if(this->parentChunkId == 0x3000000)
	{
		switch(chunkHeader.type)
		{
			case 1:
				this->GetCurrentStyleElementData().unknownChordTable = UnknownChunk(chunkHeader, dataReader.InputStream());
				break;
			case 2:
				ASSERT_EQUALS(0, chunkHeader.version.major);
				ASSERT((chunkHeader.version.minor == 3)
					|| (chunkHeader.version.minor == 4)
					,String::Number(chunkHeader.version.minor));

				this->ReadStyleTrackDataChunk(dataReader);
				break;
		}
	}
}

void StyleReader::OnEnteringChunk(const ChunkHeader& chunkHeader)
{
	this->parentChunkId = chunkHeader.id;

	if(chunkHeader.id == 0x3000000)
	{
		this->nextMIDITrackNumber = 0;
	}
}

void StyleReader::OnLeavingChunk(const ChunkHeader& chunkHeader)
{
	if(chunkHeader.id == 0x3000000)
	{
		this->currentStyleElementNumber++;
	}
}

//Private methods
void StyleReader::Read0x1000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->data._0x1000008_chunk;

	chunk.highest = dataReader.ReadUInt16();

	uint16 nEntries = dataReader.ReadUInt16();
	chunk.values.Resize(nEntries);

	for(uint16 i = 0; i < nEntries; i++)
	{
		uint16 v = dataReader.ReadUInt16();
		ASSERT(v <= chunk.highest, String::HexNumber(v));
		chunk.values[i] = v;
	}
}

void StyleReader::Read0x1000308Chunk(DataReader &dataReader)
{
	auto& chunk = this->data._0x1000308_chunk;

	uint8 nameLength = dataReader.ReadByte();
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	chunk.name = textReader.ReadString(nameLength);

	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadByte();
	chunk.unknown4 = dataReader.ReadByte();

	chunk.unknown5 = dataReader.ReadByte();
	ASSERT((chunk.unknown5 == 1)
		   || (chunk.unknown5 == 0xff), String::HexNumber(chunk.unknown5));

	chunk.unknown6 = dataReader.ReadByte();
	chunk.unknown7 = dataReader.ReadByte();
	chunk.unknown8 = dataReader.ReadByte();

	chunk.unknown9 = dataReader.ReadByte();
	chunk.unknown10 = dataReader.ReadByte();
	chunk.unknown11 = dataReader.ReadByte();
	chunk.unknown12 = dataReader.ReadByte();
	chunk.unknown13 = dataReader.ReadByte();
	chunk.unknown14 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x7F, dataReader.ReadByte());
	ASSERT_EQUALS(0xFF, dataReader.ReadByte());
}

void StyleReader::Read0x2000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = libKORG::MIDI_Track::CHUNK_0x2000008;

	midiTrack._0x2000008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x2000008_data.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);
}

void StyleReader::Read0x20000FDChunk(DataReader &dataReader)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me for reading!!!!!

	ASSERT_EQUALS(0, dataReader.ReadUInt16());
	ASSERT_EQUALS(18, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadUInt16());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	NOT_IMPLEMENTED_ERROR;
	//this->ReadKORG_MIDIEvents(dataLength, dataReader);
}

void StyleReader::Read0x3000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = (this->parentChunkId == 0x2000000) ? this->GetNextMIDITrack() : this->GetCurrentStyleElementTrack();
	midiTrack.chunkType = libKORG::MIDI_Track::CHUNK_0x3000008;

	midiTrack._0x3000008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x3000008_data.unknown2 = dataReader.ReadByte();
	midiTrack._0x3000008_data.unknown3 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	if(this->parentChunkId == 0x2000000)
	{
		midiTrack._0x3000008_data.unknown4 = dataReader.ReadByte();
		midiTrack._0x3000008_data.unknown5 = dataReader.ReadByte();
	}
	else
	{
		auto&cv = this->GetCurrentChordVariationData();

		uint8 nAdditionals = dataReader.ReadByte();
		cv.unknown.Resize(nAdditionals);
		for (uint8 i = 0; i < nAdditionals; i++)
		{
			cv.unknown[i] = dataReader.ReadUInt16();
		}

		this->nextMIDITrackNumber++;
	}
}

void StyleReader::Read0x4000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = libKORG::MIDI_Track::CHUNK_0x4000008;

	midiTrack._0x4000008_data.unknown1 = dataReader.ReadByte();
	midiTrack._0x4000008_data.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	midiTrack._0x4000008_data.unknown3 = dataReader.ReadByte();
	midiTrack._0x4000008_data.unknown4 = dataReader.ReadByte();
}

void StyleReader::Read0x5010008Chunk(DataReader &dataReader)
{
	auto& midiTrack = this->GetNextMIDITrack();
	midiTrack.chunkType = libKORG::MIDI_Track::CHUNK_0x5010008;

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

void StyleReader::ReadKORG_MIDIEvents(uint16 dataLength, DynamicArray<KORG_MIDI_Event>& midiEvents, DataReader &dataReader)
{
	midiEvents.EnsureCapacity(dataLength * 2 / 3); //Events are either 1 or 2 bytes, with the exception being 0x40 and end of track marker

	bool foundEndOfMarker = false;
	while(!foundEndOfMarker && dataLength)
	{
		uint8 eventType = dataReader.ReadByte();
		dataLength--;

		if(eventType & 0x80)
		{
			uint8 length = eventType & ~0x80;

			midiEvents.Push({KORG_MIDI_EventType::DeltaTime, length});
			continue;
		}
		else if(eventType & 0x40)
		{
			//uint8 unknown1 = dataReader.ReadByte();
			ASSERT((eventType == 0x40)
				   || (eventType == 0x41)
				   || (eventType == 0x43)
				   || (eventType == 0x46), String::HexNumber(eventType));

			uint8 unknown2 = dataReader.ReadByte();
			ASSERT((unknown2 == 0x8)
				   || (unknown2 == 0x9)
				   || (unknown2 == 0xA)
				   || (unknown2 == 0xB)
				   || (unknown2 == 0xC)
				   || (unknown2 == 0xD)
				   || (unknown2 == 0xE)
				   || (unknown2 == 0xF)
				   || (unknown2 == 0x71)
				   || (unknown2 == 0x72)
				   || (unknown2 == 0x73)
				   || (unknown2 == 0x74)
				   || (unknown2 == 0x75)
				   || (unknown2 == 0x76)
				   || (unknown2 == 0x77)
				   || (unknown2 == 0x78), String::HexNumber(unknown2));
			dataLength--;

			eventType &= ~0x40;
		}

		switch(eventType)
		{
			case 0: //probably note off
			case 1: //probably note on
			{
				uint8 value1 = dataReader.ReadByte();

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				midiEvents.Push({eventType == 1 ? KORG_MIDI_EventType::NoteOn : KORG_MIDI_EventType::NoteOff, value1, value2});

				//if(eventType == 1)
				//stdOut << u8"note " << PitchToString(value1) << " " << (uint32)value2 << endl;

				dataLength -= 2;
			}
			break;
			case 3:
			{
				uint8 value1 = dataReader.ReadByte();
				ASSERT_EQUALS(0, value1 & 0x80);

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				midiEvents.Push({KORG_MIDI_EventType::ControlChange, value1, value2});
				//stdOut << u8"control " << (uint16)value1 << " " << (uint32)value2 << endl;

				dataLength -= 2;
			}
			break;
			case 5:
			{
				uint8 value1 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value1 & 0x80);
				value1 &= ~0x80;

				midiEvents.Push({KORG_MIDI_EventType::Aftertouch, value1});
				//stdOut << u8"aftertouch " << (uint16)value1 << endl;

				dataLength--;
			}
			break;
			case 6:
			{
				uint8 b1 = dataReader.ReadByte();
				ASSERT_EQUALS(0, b1 & 0x80);

				uint8 b2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, b2 & 0x80);
				b2 &= ~0x80;

				uint16 combined = (b2 << 7) | b1;

				int16 value1 = -8192 + combined;

				midiEvents.Push({KORG_MIDI_EventType::Bend, static_cast<uint16>(value1)});
				//stdOut << u8"bend " << value1 << endl;

				dataLength -= 2;
			}
				break;
			case 9: //probably end of track marker
			{
				uint8 unknown = dataReader.ReadByte();
				dataLength--;

				if(unknown == 0x7E)
				{
					uint16 nBytes = dataReader.ReadUInt16();
					ASSERT((nBytes == 2)
						   || (nBytes == 4), String::HexNumber(nBytes));

					DynamicArray<uint8> bytes;
					bytes.Resize(nBytes);
					dataReader.ReadBytes(&bytes[0], nBytes);
					dataLength -= 2 + nBytes;

					midiEvents.Push({KORG_MIDI_EventType::Unknown, nBytes, 0, Move(bytes)});
					//stdOut << u8"unknown " << nBytes << endl;
					break;
				}

				ASSERT_EQUALS(0x2F, unknown);
				ASSERT_EQUALS(0, dataReader.ReadUInt16());
				dataLength -= 2;

				midiEvents.Push({KORG_MIDI_EventType::EndOfTrack});

				foundEndOfMarker = true;
			}
				break;
			case 12:
			case 13:
			{
				uint8 value1 = dataReader.ReadByte();

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				midiEvents.Push({eventType == 13 ? KORG_MIDI_EventType::RXnoiseOn : KORG_MIDI_EventType::RXnoiseOff, value1, value2});
				//if(eventType == 13)
				//stdOut << u8"rxnoise " << PitchToString(value1) << " " << (uint32)value2 << endl;

				dataLength -= 2;
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
}

StyleTrackData StyleReader::ReadStyleTrackData(DataReader &dataReader)
{
	StyleTrackData styleTrackData;

	styleTrackData.expression = dataReader.ReadByte();
	styleTrackData.soundProgramChangeSeq = new ProgramChangeSequence(ProgramChangeSequence::Read(dataReader));
	styleTrackData.keyboardRangeBottom = static_cast<Pitch>(dataReader.ReadByte());
	styleTrackData.keyboardRangeTop = static_cast<Pitch>(dataReader.ReadByte());

	return styleTrackData;
}

void StyleReader::ReadStyleTrackDataChunk(DataReader &dataReader)
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
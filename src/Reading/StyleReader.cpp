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
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
libKORG::Style *StyleReader::TakeResult()
{
	return new Style();
}

//Protected methods
String StyleReader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/STYLE/";
}

void StyleReader::ReadDataChunk(uint32 chunkId, DataReader &dataReader)
{
	switch(chunkId)
	{
		case 0x1000008:
			this->Read0x1000008Chunk(dataReader);
			break;
		case 0x1000308:
			this->Read0x1000308Chunk(dataReader);
			break;
		case 0x1010108:
			this->Read0x1010108Chunk(dataReader);
			break;
		case 0x2000008:
			this->Read0x2000008Chunk(dataReader);
			break;
		case 0x2000308:
			this->Read0x2000308Chunk(dataReader);
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

void StyleReader::OnEnteringChunk(uint32 chunkId)
{
	this->parentChunkId = chunkId;
}

void StyleReader::OnLeavingChunk(uint32 chunkId)
{
}

//Private methods
void StyleReader::Read0x1000008Chunk(DataReader &dataReader)
{
	uint16 highest = dataReader.ReadUInt16();
	uint16 nEntries = dataReader.ReadUInt16();

	for(uint16 i = 0; i < nEntries; i++)
	{
		uint16 v = dataReader.ReadUInt16();
		ASSERT(v <= highest, String::HexNumber(v));
	}
}

void StyleReader::Read0x1000308Chunk(DataReader &dataReader)
{
	uint8 nameLength = dataReader.ReadByte();
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	String name = textReader.ReadString(nameLength);
	//stdOut << name << endl;

	uint8 unknown1 = dataReader.ReadByte();
	uint8 unknown11 = dataReader.ReadByte();
	uint8 unknown12 = dataReader.ReadByte();

	uint8 unknown2 = dataReader.ReadByte();
	ASSERT((unknown2 == 0)
		   || (unknown2 == 0x1f)
		   || (unknown2 == 0x7f), String::HexNumber(unknown2));

	uint8 unknown3 = dataReader.ReadByte();
	ASSERT((unknown3 == 1)
		   || (unknown3 == 0xff), String::HexNumber(unknown3));

	uint8 unknown31 = dataReader.ReadByte();
	uint8 unknown33 = dataReader.ReadByte();
	uint8 unknown32 = dataReader.ReadByte();

	uint8 unknown4 = dataReader.ReadByte();
	uint8 unknown41 = dataReader.ReadByte();
	uint8 unknown42 = dataReader.ReadByte();
	uint8 unknown43 = dataReader.ReadByte();
	uint8 unknown5 = dataReader.ReadByte();
	uint8 unknown6 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x7F, dataReader.ReadByte());
	ASSERT_EQUALS(0xFF, dataReader.ReadByte());
}

void StyleReader::Read0x1010108Chunk(DataReader &dataReader)
{
	uint8 unknown1 = dataReader.ReadByte();
	ASSERT((unknown1 == 3)
		   || (unknown1 == 0x3f), String::HexNumber(unknown1));

	uint8 unknown3 = dataReader.ReadByte();

	ASSERT_EQUALS(0x18, dataReader.ReadByte());
	dataReader.Skip(0x18); //chord table

	uint8 unknown2 = dataReader.ReadByte();
	ASSERT((unknown2 == 0)
		   || (unknown2 == 1)
		   || (unknown2 == 2)
		   || (unknown2 == 3), String::HexNumber(unknown1));
}

void StyleReader::Read0x2000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 unknown1 = dataReader.ReadByte();
	uint8 unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, dataReader);
}

void StyleReader::Read0x2000308Chunk(DataReader &dataReader)
{
	StyleTrackData styleTrackData[8];

	styleTrackData[0] = this->ReadStyleTrackData(dataReader); //drums
	styleTrackData[1] = this->ReadStyleTrackData(dataReader); //percussion
	styleTrackData[2] = this->ReadStyleTrackData(dataReader); //bass
	styleTrackData[3] = this->ReadStyleTrackData(dataReader); //acc1
	styleTrackData[4] = this->ReadStyleTrackData(dataReader); //acc2
	styleTrackData[5] = this->ReadStyleTrackData(dataReader); //acc3
	styleTrackData[6] = this->ReadStyleTrackData(dataReader); //acc4
	styleTrackData[7] = this->ReadStyleTrackData(dataReader); //acc5

	ASSERT_EQUALS(0, dataReader.ReadUInt16());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown115 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown116 = dataReader.ReadByte();
	uint8 unknown114 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown111 = dataReader.ReadByte();
	uint8 unknown112 = dataReader.ReadByte();
	uint8 unknown113 = dataReader.ReadByte();
	uint8 unknown17 = dataReader.ReadByte();
	uint8 unknown13 = dataReader.ReadByte();
	uint8 unknown14 = dataReader.ReadByte();
	uint8 unknown11 = dataReader.ReadByte();
	uint8 unknown15 = dataReader.ReadByte();
	uint8 unknown16 = dataReader.ReadByte();
	uint8 unknown12 = dataReader.ReadByte();
	uint8 unknown18 = dataReader.ReadByte();
	uint8 unknown181 = dataReader.ReadByte();
	uint8 unknown19 = dataReader.ReadByte();
	uint8 unknown22 = dataReader.ReadByte();
	uint8 unknown20 = dataReader.ReadByte();
	uint8 unknown21 = dataReader.ReadByte();

	for(uint8 i = 0; i < 16; i++)
	{
		uint8 unknown4 = dataReader.ReadByte();
		ASSERT((unknown4 == 0)
			|| (unknown4 == 1)
			|| (unknown4 == 2)
			|| (unknown4 == 3)
			|| (unknown4 == 4)
			|| (unknown4 == 0x14)
			|| (unknown4 == 0x20)
			|| (unknown4 == 0x23)
		, String::HexNumber(unknown4));
	}
}

void StyleReader::Read0x3000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 unknown1 = dataReader.ReadByte();

	uint8 unknown21 = dataReader.ReadByte();
	uint8 unknown22 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, dataReader);

	if(parentChunkId == 0x2000000)
	{
		uint8 unknown2 = dataReader.ReadByte();
		uint8 unknown3 = dataReader.ReadByte();
	}
	else
	{
		uint8 nAdditionals = dataReader.ReadByte();
		for (uint8 i = 0; i < nAdditionals; i++)
		{
			dataReader.ReadUInt16();
		}
	}
}

void StyleReader::Read0x4000008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 unknown1 = dataReader.ReadByte();
	uint8 unknown11 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, dataReader);

	uint8 unknown2 = dataReader.ReadByte();
	uint8 unknown3 = dataReader.ReadByte();
}

void StyleReader::Read0x5010008Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 unknown1 = dataReader.ReadByte();
	uint8 unknown11 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, dataReader);

	uint8 unknown2 = dataReader.ReadByte();
	uint8 unknown3 = dataReader.ReadByte();
}

void StyleReader::ReadKORG_MIDIEvents(uint16 dataLength, DataReader &dataReader)
{
	//TODO: DO THIS CORRECTLY AGAIN
	dataReader.Skip(dataLength);
	return;

	DynamicArray<KORG_MIDI_Event> midiEvents;

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

		switch(eventType)
		{
			case 0: //probably note off
			case 1: //probably note on
			{
				Pitch value1 = static_cast<Pitch>(dataReader.ReadByte());

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				midiEvents.Push({eventType == 1 ? KORG_MIDI_EventType::NoteOn : KORG_MIDI_EventType::NoteOff, (uint16)value1, value2});
				if(eventType == 1)
				stdOut << u8"note " << (uint16)value1 << " " << (uint32)value2 << endl;

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

				midiEvents.Push({KORG_MIDI_EventType::Ctrl, value1, value2});

				dataLength -= 2;
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
					dataReader.Skip(nBytes);
					dataLength -= 2 + nBytes;

					midiEvents.Push({KORG_MIDI_EventType::Unknown, nBytes});
					break;
				}

				ASSERT_EQUALS(0x2F, unknown);
				ASSERT_EQUALS(0, dataReader.ReadUInt16());
				dataLength -= 2;
				ASSERT_EQUALS(0, dataLength);

				midiEvents.Push({KORG_MIDI_EventType::EndOfTrack});

				foundEndOfMarker = true;
			}
				break;
			case 12:
			case 13:
			{
				Pitch value1 = static_cast<Pitch>(dataReader.ReadByte());

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				midiEvents.Push({eventType == 13 ? KORG_MIDI_EventType::RXnoiseOn : KORG_MIDI_EventType::RXnoiseOff, (uint16)value1, value2});

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
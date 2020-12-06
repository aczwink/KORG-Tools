/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "PerformanceReader.hpp"
//Local
#include <libkorg/ProgramChangeSequence.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

const int8 c_knob_offset = 64;

//Protected methods
String PerformanceReader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/PERFORMANCE/";
}

void PerformanceReader::ReadDataChunk(uint32 chunkId, DataReader &dataReader)
{
	switch(chunkId)
	{
		case 0x06000008:
			ASSERT_EQUALS(0, dataReader.ReadUInt16());
			ASSERT_EQUALS(true, dataReader.InputStream().IsAtEnd());
			break;
		case 0x07000008:
			this->Read0x07000008Chunk(dataReader);
			break;
		case 0x08000008:
			this->Read0x08000008Chunk(dataReader);
			break;
		case 0x09000008:
			this->Read0x09000008Chunk(dataReader);
			break;
		case 0x0F000008:
		{
			ASSERT_EQUALS(4, dataReader.ReadUInt32());
			TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
			stdOut << textReader.ReadZeroTerminatedString(41) << endl;
		}
		break;
		case 0x15000108:
		{
			uint32 trackNumber = dataReader.ReadUInt32();
			ASSERT(trackNumber < 4, String::Number(trackNumber));

			uint32 unknown1 = dataReader.ReadUInt32();
			ASSERT((unknown1 == 0xb3)
				   || (unknown1 == 0xb4)
				   || (unknown1 == 0)
			, String::Number(unknown1));
		}
		break;
	}
}

//Private methods
void PerformanceReader::Read0x07000008Chunk(DataReader &dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();
	ASSERT(trackNumber < 6, String::Number(trackNumber));

	uint8 unknown1 = dataReader.ReadByte();
	ASSERT((unknown1 == 0)
		|| (unknown1 == 1)
		|| (unknown1 == 2)
		|| (unknown1 == 3)
		|| (unknown1 == 4)
		|| (unknown1 == 5)
		|| (unknown1 == 6)
		|| (unknown1 == 7)
		, String::Number(unknown1));

	uint8 unknown2 = dataReader.ReadByte();
	ASSERT((unknown2 == 0)
		   || (unknown2 == 1)
	, String::Number(unknown2));

	uint8 unknown3 = dataReader.ReadByte();
	ASSERT((unknown3 == 0)
		   || (unknown3 == 1)
	, String::Number(unknown2));

	uint8 unknown4 = dataReader.ReadByte();
	ASSERT((unknown4 == 0xff)
		   || (unknown4 == 4)
		   || (unknown4 == 5)
		   || (unknown4 == 7)
		   || (unknown4 == 1)
	, String::Number(unknown4));

	ASSERT_EQUALS(0xff, dataReader.ReadUInt16());

	uint8 unknown5 = dataReader.ReadByte();
	ASSERT((unknown5 == 0xff)
		   || (unknown5 == 1)
	, String::Number(unknown5));

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 unknown6 = dataReader.ReadByte();
	ASSERT((unknown6 == 0x4a)
		   || (unknown6 == 0x47)
		   || (unknown6 == 0x0)
		   || (unknown6 == 0x1a)
	, String::Number(unknown6));

	uint8 unknown7 = dataReader.ReadByte();
	ASSERT((unknown7 == 0x3c)
		   || (unknown7 == 0x28)
		   || (unknown7 == 0x0)
		   || (unknown7 == 0x64)
	, String::Number(unknown7));

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 unknown8 = dataReader.ReadByte();
	ASSERT((unknown8 == 0xd9)
		   || (unknown8 == 0)
		   || (unknown8 == 0xf9)
		   || (unknown8 == 0x32)
	, String::Number(unknown8));

	uint8 unknown9 = dataReader.ReadByte();
	ASSERT((unknown9 == 0xe9)
		   || (unknown9 == 0xe3)
		   || (unknown9 == 0)
		   || (unknown9 == 0xf2)
		   || (unknown9 == 9)
	, String::Number(unknown9));

	uint8 unknown10 = dataReader.ReadByte();
	ASSERT((unknown10 == 0x64)
		   || (unknown10 == 0xe1)
		   || (unknown10 == 0)
		   || (unknown10 == 0xe0)
		   || (unknown10 == 0x40)
	, String::Number(unknown10));

	uint8 unknown11 = dataReader.ReadByte();
	ASSERT((unknown11 == 6)
		   || (unknown11 == 0xa1)
		   || (unknown11 == 0)
	, String::Number(unknown11));

	uint8 unknown12 = dataReader.ReadByte();
	ASSERT((unknown12 == 0x46)
		   || (unknown12 == 0)
	, String::Number(unknown12));

	uint8 unknown13 = dataReader.ReadByte();
	ASSERT((unknown13 == 0xa8)
		   || (unknown13 == 0)
	, String::Number(unknown13));

	uint8 unknown14 = dataReader.ReadByte();
	ASSERT((unknown14 == 0x8d)
		   || (unknown14 == 0)
		   || (unknown14 == 0xee)
	, String::Number(unknown14));

	uint8 unknown15 = dataReader.ReadByte();
	ASSERT((unknown15 == 2)
		   || (unknown15 == 0)
		   || (unknown15 == 0xb2)
	, String::Number(unknown15));

	uint8 unknown16 = dataReader.ReadByte();
	ASSERT((unknown16 == 0x60)
		   || (unknown16 == 0)
	, String::Number(unknown16));

	uint8 unknown17 = dataReader.ReadByte();
	ASSERT((unknown17 == 0xce)
		   || (unknown17 == 0)
	, String::Number(unknown17));

	uint8 unknown18 = dataReader.ReadByte();
	ASSERT((unknown18 == 0xd)
		   || (unknown18 == 0)
	, String::Number(unknown18));

	uint8 unknown19 = dataReader.ReadByte();
	ASSERT((unknown19 == 5)
		   || (unknown19 == 0)
	, String::Number(unknown19));

	ASSERT_EQUALS(0, dataReader.ReadUInt64());
	ASSERT_EQUALS(0, dataReader.ReadUInt64());
	ASSERT_EQUALS(0, dataReader.ReadUInt64());
	ASSERT_EQUALS(0, dataReader.ReadUInt64());
	ASSERT_EQUALS(0, dataReader.ReadUInt64());
	ASSERT_EQUALS(0, dataReader.ReadUInt64());
	ASSERT_EQUALS(0, dataReader.ReadByte());
}

void PerformanceReader::Read0x08000008Chunk(DataReader &dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();
	ASSERT(trackNumber < 4, String::Number(trackNumber));

	ASSERT_EQUALS(9, dataReader.ReadByte());

	uint8 unknown1 = dataReader.ReadByte();
	ASSERT((unknown1 == 0xc)
		   || (unknown1 == 0xb)
		   || (unknown1 == 0xf)
		   || (unknown1 == 0x12)
		   || (unknown1 == 0)
		   || (unknown1 == 1)
	, String::Number(unknown1));

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown2 = dataReader.ReadByte();
	ASSERT((unknown2 == 2)
		|| (unknown2 == 0xd)
		|| (unknown2 == 0xb)
		|| (unknown2 == 0xa)
		|| (unknown2 == 0x4)
		|| (unknown2 == 0x1d)
		|| (unknown2 == 0xf)
		|| (unknown2 == 0x10)
	, String::HexNumber(unknown2));

	uint8 unknown3 = dataReader.ReadByte();
	ASSERT((unknown3 == 0x3e)
		|| (unknown3 == 0x45)
		|| (unknown3 == 0x61)
		|| (unknown3 == 0x3f)
		|| (unknown3 == 0x5e)
		|| (unknown3 == 0x6e)
	, String::HexNumber(unknown3));

	uint8 unknown4 = dataReader.ReadByte();
	ASSERT((unknown4 == 0x40)
		|| (unknown4 == 0x3b)
		|| (unknown4 == 0x7f)
	, String::HexNumber(unknown4));

	uint8 unknown5 = dataReader.ReadByte();
	ASSERT((unknown5 == 0xf)
		|| (unknown5 == 0x36)
		|| (unknown5 == 0x25)
		|| (unknown5 == 0x50)
	, String::HexNumber(unknown5));

	ASSERT_EQUALS(0, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
}

void PerformanceReader::Read0x09000008Chunk(DataReader& dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();
	ASSERT(trackNumber < 8, "???");

	uint16 unknown_101 = dataReader.ReadUInt16();
	ASSERT((unknown_101 == 2)
		   || (unknown_101 == 3), "???");

	uint8 unknown_103 = dataReader.ReadByte();
	ASSERT((unknown_103 == 3)
		   || (unknown_103 == 7), "???");

	uint8 muted = dataReader.ReadByte();
	ASSERT((muted == 0)
		   || (muted == 4), "???"); //0 = playing, 4 = muted

	ASSERT_EQUALS(0x7F, dataReader.ReadUInt16());

	uint16 unknown1 = dataReader.ReadUInt16();
	ASSERT((unknown1 == 0x156C)
		   || (unknown1 == 0x7F), "???");

	ProgramChangeSequence soundProgramChangeSeq = ProgramChangeSequence::FromEncoded(dataReader.ReadUInt32());

	uint8 volume = dataReader.ReadByte();
	int8 pan = dataReader.ReadByte() - c_knob_offset;
	int8 detune = dataReader.ReadByte() - c_knob_offset;

	int8 octaveTranspose = (int8)dataReader.ReadByte() / 12;

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	uint8 unknown4 = dataReader.ReadByte();
	ASSERT((unknown4 == 0x40)
		   || (unknown4 == 0x63)
		   //styleperf:
		   || (unknown4 == 0x48)
		   || (unknown4 == 0x56)
	, String::HexNumber(unknown4));

	uint8 unknown5 = dataReader.ReadByte();
	ASSERT((unknown5 == 0x40)
		   || (unknown5 == 0x2E)
		   //styleperf:
		   || (unknown5 == 0x54), String::HexNumber(unknown5));

	uint8 unknown6 = dataReader.ReadByte();
	ASSERT((unknown6 == 0x40)
		   || (unknown6 == 0x3A), "???");

	uint8 unknown7 = dataReader.ReadByte();
	ASSERT((unknown7 == 0x40)
		   || (unknown7 == 0x4D), "???");

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	uint8 unknown8 = dataReader.ReadByte();
	ASSERT((unknown8 == 0)
		   || (unknown8 == 1), "???");

	uint8 unknown9 = dataReader.ReadUInt16();
	ASSERT((unknown9 == 0)
		   || (unknown9 == 1), "???");

	uint8 fxMaster1 = dataReader.ReadByte();
	uint8 fxMaster2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	uint8 dry = dataReader.ReadByte();
	ASSERT(dry <= 1, "??");

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown12 = dataReader.ReadByte();
	ASSERT((unknown12 == 1)
		   || (unknown12 == 0x41), "???");

	uint8 unknown13 = dataReader.ReadByte();
	ASSERT((unknown13 == 4)
		   || (unknown13 == 7), "???");

	uint8 pbSensitivity = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	int8 lowGainTimes2 = dataReader.ReadByte();
	int8 midGainTimes2 = dataReader.ReadByte();
	int8 highGainTimes2 = dataReader.ReadByte();

	uint8 unknown18 = dataReader.ReadByte();
	ASSERT((unknown18 == 0x61)
		   || (unknown18 == 0x5a), "???");

	ASSERT_EQUALS(0xFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFFFFFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFFFFFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFFFFFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFF, dataReader.ReadUInt16());
	ASSERT_EQUALS(0xff, dataReader.ReadByte());

	uint8 unknown19 = dataReader.ReadByte();
	ASSERT((unknown19 == 0x72)
		   || (unknown19 == 0x7f), "???");

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	uint8 unknown20 = dataReader.ReadByte();
	ASSERT((unknown20 == 0x76)
		   || (unknown20 == 0x7f), "???");

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x7f, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	uint8 unknown21 = dataReader.ReadByte();
	ASSERT((unknown21 == 0x64)
		   || (unknown21 == 0x7F), "???");

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x7f, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x7f, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x7f, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x7f, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadUInt32());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	/*
	stdOut
		<< "101\t" << unknown_101 << endl
		<< "103\t" << unknown_103 << endl
		<< "1\t" << unknown1 << endl
		<< "4\t" << unknown4 << endl
		<< "5\t" << unknown5 << endl
		<< "6\t" << unknown6 << endl
		<< "7\t" << unknown7 << endl
		<< "8\t" << unknown8 << endl
		<< "9\t" << unknown9 << endl
		<< "12\t" << unknown12 << endl
		<< "13\t" << unknown13 << endl
		<< "18\t" << unknown18 << endl
		<< "19\t" << unknown19 << endl
		<< "20\t" << unknown20 << endl
		<< "21\t" << unknown21 << endl
	;*/
}
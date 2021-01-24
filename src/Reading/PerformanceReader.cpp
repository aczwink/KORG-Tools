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
#include "PerformanceReader.hpp"
//Local
#include <libkorg/ProgramChangeSequence.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

const int8 c_knob_offset = 64;

//Public methods
libKORG::Performance *PerformanceReader::TakePerformanceResult()
{
	ASSERT_EQUALS(1, this->perfIndex);

	return new Performance(Move(this->accompanimentSettings), Move(this->keyboardSettings[0]));
}

libKORG::SingleTouchSettings *PerformanceReader::TakeSTSResult()
{
	ASSERT_EQUALS(4, this->perfIndex);

	return new SingleTouchSettings(Move(this->accompanimentSettings), Move(this->keyboardSettings));
}

//Protected methods
String PerformanceReader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/PERFORMANCE/";
}

void PerformanceReader::ReadDataChunk(uint32 chunkId, DataReader &dataReader)
{
	switch(chunkId)
	{
		case 0x04000108:
			this->Read0x04000108Chunk(dataReader);
			break;
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
			this->Read0x0F000008Chunk(dataReader);
		break;
		case 0x10000008:
			this->Read0x10000008Chunk(dataReader);
		break;
		case 0x11000008:
		{
			uint16 unknownWhatever = dataReader.ReadUInt16();
			ASSERT((unknownWhatever == 0xFF)
				   || (unknownWhatever == 0), String::HexNumber(unknownWhatever));
		}
		break;
		case 0x12000108:
			this->Read0x12000108Chunk(dataReader);
			break;
		case 0x13000008:
		{
			uint8 unknownWhatever = dataReader.ReadByte();

			ASSERT_EQUALS(0, dataReader.ReadByte());
			ASSERT_EQUALS(0, dataReader.ReadByte());

			uint8 unknown1 = dataReader.ReadByte();

			ASSERT_EQUALS(0, dataReader.ReadUInt32());

			uint8 unknown2 = dataReader.ReadByte();

			ASSERT_EQUALS(0, dataReader.ReadByte());
			ASSERT_EQUALS(0, dataReader.ReadUInt16());

			uint8 unknown3 = dataReader.ReadByte();
			uint8 unknown4 = dataReader.ReadByte();
		}
		break;
		case 0x14000008:
		{
			ASSERT_EQUALS(0, dataReader.ReadByte());

			uint8 unknown3 = dataReader.ReadByte();
			ASSERT((unknown3 == 0)
				   || (unknown3 == 4), String::HexNumber(unknown3));

			uint8 unknown2 = dataReader.ReadByte();
			ASSERT((unknown2 == 0)
				|| (unknown2 == 1)
				|| (unknown2 == 2), String::HexNumber(unknown2));

			uint8 unknown1 = dataReader.ReadByte();
			ASSERT((unknown1 == 0)
				|| (unknown1 == 1), String::HexNumber(unknown1));

			ASSERT_EQUALS(0, dataReader.ReadByte());
		}
		break;
		case 0x15000108:
		{
			uint32 trackNumber = dataReader.ReadUInt32();
			ASSERT(trackNumber < 4, String::Number(trackNumber));

			uint32 unknown1 = dataReader.ReadUInt32();
			ASSERT((unknown1 == 0xb3)
				   || (unknown1 == 0xb4)
				   || (unknown1 == 0x6B)
				   || (unknown1 == 0x43)
				   || (unknown1 == 0x42)
				   || (unknown1 == 0x32)
				   || (unknown1 == 0x1d)
				   || (unknown1 == 0)
			, String::HexNumber(unknown1));
		}
		break;
		case 0x18000008:
		{
			uint16 unknown1 = dataReader.ReadUInt16();
			ASSERT((unknown1 == 3)
				   || (unknown1 == 7)
				   || (unknown1 == 8), String::HexNumber(unknown1));

			uint16 unknown2 = dataReader.ReadUInt16();
			ASSERT((unknown2 == 0)
				   || (unknown2 == 3)
				   || (unknown2 == 7), String::HexNumber(unknown2));

			uint16 unknown3 = dataReader.ReadUInt16();
			ASSERT((unknown3 == 0)
				   || (unknown3 == 5)
				   || (unknown3 == 6), String::HexNumber(unknown3));

			uint16 unknown4 = dataReader.ReadUInt16();
			ASSERT((unknown4 == 0)
				   || (unknown4 == 5), String::HexNumber(unknown4));

			ASSERT_EQUALS(0, dataReader.ReadUInt64());
			ASSERT_EQUALS(0, dataReader.ReadUInt16());

			uint8 unknown5 = dataReader.ReadByte();
			ASSERT((unknown5 == 0)
				   || (unknown5 == 1)
				   || (unknown5 == 2), String::HexNumber(unknown5));

			uint16 unknown6 = dataReader.ReadUInt16();
			uint16 unknown7 = dataReader.ReadUInt16();

			uint16 unknown8 = dataReader.ReadUInt16();
			ASSERT((unknown8 == 0)
				   || (unknown8 == 1), String::HexNumber(unknown8));

			uint8 unknown9 = dataReader.ReadByte();
			ASSERT((unknown9 == 0)
				   || (unknown9 == 1), String::HexNumber(unknown9));

			ASSERT_EQUALS(0x01360119, dataReader.ReadUInt32());
		}
		break;
		case 0x1A000008:
		{
			ASSERT_EQUALS(1, dataReader.ReadByte());
			ASSERT_EQUALS(0x3C, dataReader.ReadByte());
		}
		break;
		case 0x1B000008:
		{
			ASSERT_EQUALS(1, dataReader.ReadByte());
			ASSERT_EQUALS(0xff, dataReader.ReadByte());

			uint8 unknown1 = dataReader.ReadByte();
			ASSERT((unknown1 == 0)
				   || (unknown1 == 0xff), String::HexNumber(unknown1));
		}
		break;
	}
}

void PerformanceReader::OnEnteringChunk(uint32 chunkId)
{
	this->parentChunkId = chunkId;
}

void PerformanceReader::OnLeavingChunk(uint32 chunkId)
{
	switch(chunkId)
	{
		case 0xA000000:
			this->perfIndex++;
			break;
	}
}

//Private methods
void PerformanceReader::Read0x04000108Chunk(DataReader &dataReader)
{
	ASSERT_EQUALS(0xf, dataReader.ReadUInt32());

	uint8 unknown1 = dataReader.ReadByte();
	ASSERT((unknown1 == 0)
		|| (unknown1 == 1)
		|| (unknown1 == 2)
		|| (unknown1 == 3)
		|| (unknown1 == 4)
		|| (unknown1 == 5)
		|| (unknown1 == 6)
		|| (unknown1 == 7)
		|| (unknown1 == 8)
		|| (unknown1 == 9)
		|| (unknown1 == 0xA)
		|| (unknown1 == 0xB)
		|| (unknown1 == 0xC)
		|| (unknown1 == 0xD)
		|| (unknown1 == 0xE)
		|| (unknown1 == 0x11)
		|| (unknown1 == 0x12)
		|| (unknown1 == 0x13), String::HexNumber(unknown1));

	uint8 unknown11 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown2 = dataReader.ReadByte();

	uint8 unknown3 = dataReader.ReadByte();
	ASSERT((unknown3 == 0)
		   || (unknown3 == 1)
		   || (unknown3 == 2)
		   || (unknown3 == 3)
	, String::HexNumber(unknown3));

	uint8 unknown4 = dataReader.ReadByte();
	ASSERT((unknown4 == 0)
		   || (unknown4 == 1)
		   || (unknown4 == 2)
		   || (unknown4 == 3)
		   || (unknown4 == 4)
		   || (unknown4 == 5)
		   || (unknown4 == 0xB)
		   || (unknown4 == 0xD)
	, String::HexNumber(unknown4));

	ASSERT_EQUALS(0, dataReader.ReadUInt32());
	ASSERT_EQUALS(0, dataReader.ReadUInt16());
	uint8 unknown52 = dataReader.ReadByte();

	uint8 unknown51 = dataReader.ReadByte();
	uint8 unknown5 = dataReader.ReadByte();
	uint8 unknown6 = dataReader.ReadByte();
	uint8 unknown7 = dataReader.ReadByte();
	uint8 unknown8 = dataReader.ReadByte();
	uint8 unknown9 = dataReader.ReadByte();
	uint8 unknown10 = dataReader.ReadByte();

	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());
	ASSERT_EQUALS(1, dataReader.ReadByte());

	uint8 unknown111 = dataReader.ReadByte();
	uint8 unknown121 = dataReader.ReadByte();
	uint8 unknown12 = dataReader.ReadByte();
	uint8 unknown122 = dataReader.ReadByte();
	uint8 unknown131 = dataReader.ReadByte();
	uint8 unknown13 = dataReader.ReadByte();
	uint8 unknown132 = dataReader.ReadByte();
	uint8 unknown141 = dataReader.ReadByte();
	uint8 unknown14 = dataReader.ReadByte();
	uint8 unknown142 = dataReader.ReadByte();
	uint8 unknown151 = dataReader.ReadByte();
	uint8 unknown15 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown161 = dataReader.ReadByte();
	uint8 unknown16 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown171 = dataReader.ReadByte();
	uint8 unknown17 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown181 = dataReader.ReadByte();
	uint8 unknown18 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown191 = dataReader.ReadByte();
	uint8 unknown19 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown201 = dataReader.ReadByte();
	uint8 unknown20 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown211 = dataReader.ReadByte();
	uint8 unknown21 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown212 = dataReader.ReadByte();
	uint8 unknown213 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown221 = dataReader.ReadByte();
	uint8 unknown22 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown231 = dataReader.ReadByte();
	uint8 unknown23 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown241 = dataReader.ReadByte();
	uint8 unknown24 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint8 unknown251 = dataReader.ReadByte();
	uint8 unknown25 = dataReader.ReadByte();
}

void PerformanceReader::Read0x07000008Chunk(DataReader &dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();
	ASSERT(trackNumber < 6, String::Number(trackNumber));

	auto& data = this->parentChunkId == 0x5000000 ? this->accompanimentSettings._0x07000008_chunks[trackNumber]
			: this->keyboardSettings[this->perfIndex]._0x07000008_chunks[trackNumber];

	data.unknown1 = dataReader.ReadByte();
	ASSERT((data.unknown1 == 0)
		|| (data.unknown1 == 1)
		|| (data.unknown1 == 2)
		|| (data.unknown1 == 3)
		|| (data.unknown1 == 4)
		|| (data.unknown1 == 5)
		|| (data.unknown1 == 6)
		|| (data.unknown1 == 7)
		, String::Number(data.unknown1));

	data.unknown2 = dataReader.ReadByte();
	ASSERT((data.unknown2 == 0)
		   || (data.unknown2 == 1)
	, String::Number(data.unknown2));

	data.unknown3 = dataReader.ReadByte();
	ASSERT((data.unknown3 == 0)
		   || (data.unknown3 == 1)
	, String::Number(data.unknown3));

	data.unknown4 = dataReader.ReadByte();
	data.unknown411 = dataReader.ReadByte();
	data.unknown412 = dataReader.ReadByte();

	data.unknown5 = dataReader.ReadByte();
	ASSERT((data.unknown5 == 0xff)
		   || (data.unknown5 == 1)
	, String::Number(data.unknown5));

	data.unknown51 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	data.unknown6 = dataReader.ReadByte();
	data.unknown7 = dataReader.ReadByte();
	data.unknown71 = dataReader.ReadByte();
	data.unknown72 = dataReader.ReadByte();
	data.unknown8 = dataReader.ReadByte();
	data.unknown9 = dataReader.ReadByte();
	data.unknown10 = dataReader.ReadByte();
	data.unknown11 = dataReader.ReadByte();
	data.unknown12 = dataReader.ReadByte();
	data.unknown13 = dataReader.ReadByte();
	data.unknown14 = dataReader.ReadByte();
	data.unknown15 = dataReader.ReadByte();
	data.unknown16 = dataReader.ReadByte();
	data.unknown17 = dataReader.ReadByte();
	data.unknown18 = dataReader.ReadByte();
	data.unknown19 = dataReader.ReadByte();
	data.unknown20 = dataReader.ReadByte();
	data.unknown21 = dataReader.ReadByte();
	data.unknown22 = dataReader.ReadByte();
	data.unknown23 = dataReader.ReadByte();
	data.unknown24 = dataReader.ReadByte();
	data.unknown251 = dataReader.ReadByte();
	data.unknown25 = dataReader.ReadByte();
	data.unknown26 = dataReader.ReadByte();
	data.unknown27 = dataReader.ReadByte();
	data.unknown28 = dataReader.ReadByte();
	data.unknown29 = dataReader.ReadByte();
	data.unknown30 = dataReader.ReadByte();
	data.unknown31 = dataReader.ReadByte();
	data.unknown32 = dataReader.ReadByte();
	data.unknown33 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());
	data.unknown331 = dataReader.ReadByte();

	data.unknown34 = dataReader.ReadByte();
	data.unknown35 = dataReader.ReadByte();
	data.unknown36 = dataReader.ReadByte();
	data.unknown37 = dataReader.ReadByte();
	data.unknown38 = dataReader.ReadByte();
	data.unknown39 = dataReader.ReadByte();
	data.unknown40 = dataReader.ReadByte();
	data.unknown41 = dataReader.ReadByte();
	data.unknown42 = dataReader.ReadByte();
	data.unknown43 = dataReader.ReadByte();
	data.unknown44 = dataReader.ReadByte();
	data.unknown45 = dataReader.ReadByte();
	data.unknown46 = dataReader.ReadByte();
	data.unknown47 = dataReader.ReadByte();
	data.unknown48 = dataReader.ReadByte();
	data.unknown49 = dataReader.ReadByte();
	data.unknown50 = dataReader.ReadByte();
	data.unknown52 = dataReader.ReadByte();
	data.unknown53 = dataReader.ReadByte();
	data.unknown54 = dataReader.ReadByte();
	data.unknown55 = dataReader.ReadByte();
	data.unknown56 = dataReader.ReadByte();
	data.unknown57 = dataReader.ReadByte();
	data.unknown58 = dataReader.ReadByte();
	data.unknown59 = dataReader.ReadByte();
	data.unknown591 = dataReader.ReadByte();
	data.unknown60 = dataReader.ReadByte();
	data.unknown61 = dataReader.ReadByte();
	data.unknown62 = dataReader.ReadByte();
	data.unknown63 = dataReader.ReadByte();
	data.unknown64 = dataReader.ReadByte();
}

void PerformanceReader::Read0x08000008Chunk(DataReader &dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();
	ASSERT(trackNumber < 4, String::Number(trackNumber));

	auto& data = this->parentChunkId == 0x5000000 ? this->accompanimentSettings._0x08000008_chunks[trackNumber]
			: this->keyboardSettings[this->perfIndex]._0x08000008_chunks[trackNumber];

	data.unknown0 = dataReader.ReadByte();
	data.unknown1 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	data.unknown2 = dataReader.ReadByte();
	data.unknown3 = dataReader.ReadByte();
	data.unknown4 = dataReader.ReadByte();
	data.unknown5 = dataReader.ReadByte();
	data.unknown6 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	data.unknown8 = dataReader.ReadByte();
	data.unknown9 = dataReader.ReadByte();
	data.unknown10 = dataReader.ReadByte();

	data.unknown7 = dataReader.ReadByte();
	ASSERT((data.unknown7 == 0x5A)
	, String::HexNumber(data.unknown7));
}

void PerformanceReader::Read0x09000008Chunk(DataReader& dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();

	TrackProperties& atp = this->parentChunkId == 0x5000000 ? this->accompanimentSettings.trackProperties[trackNumber]
			: this->keyboardSettings[this->perfIndex].trackProperties[trackNumber];

	atp.unknown_101 = dataReader.ReadUInt16();
	ASSERT((atp.unknown_101 == 2)
		   || (atp.unknown_101 == 3), "???");

	atp.unknown_103 = dataReader.ReadByte();
	ASSERT((atp.unknown_103 == 3)
		   || (atp.unknown_103 == 7), "???");

	atp.muted = dataReader.ReadByte();
	ASSERT((atp.muted == 0)
		|| (atp.muted == 1)
		|| (atp.muted == 2)
		|| (atp.muted == 4)
		|| (atp.muted == 0x40)
		|| (atp.muted == 0x44), String::HexNumber(atp.muted)); //0 = playing, 4 = muted

	atp.unknown11 = dataReader.ReadByte();
	atp.unknown111 = dataReader.ReadByte();

	atp.unknown1 = dataReader.ReadByte();
	atp.unknown113 = dataReader.ReadByte();
	atp.soundProgramChangeSeq = new ProgramChangeSequence(ProgramChangeSequence::FromEncoded(dataReader.ReadUInt32()));

	atp.volume = dataReader.ReadByte();
	atp.pan = dataReader.ReadByte() - c_knob_offset;
	atp.detune = dataReader.ReadByte() - c_knob_offset;

	atp.octaveTranspose = (int8)dataReader.ReadByte() / 12;

	atp.unknown3 = dataReader.ReadByte();
	atp.unknown4 = dataReader.ReadByte();
	atp.unknown5 = dataReader.ReadByte();
	atp.unknown6 = dataReader.ReadByte();
	atp.unknown7 = dataReader.ReadByte();
	atp.unknown71 = dataReader.ReadByte();
	atp.unknown72 = dataReader.ReadByte();
	atp.unknown73 = dataReader.ReadByte();
	atp.unknown8 = dataReader.ReadByte();

	atp.unknown9 = dataReader.ReadUInt16();
	ASSERT((atp.unknown9 == 0)
		   || (atp.unknown9 == 1), "???");

	atp.fxMaster1 = dataReader.ReadByte();
	atp.fxMaster2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	atp.dry = dataReader.ReadByte();
	ASSERT(atp.dry <= 1, "??");

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown12 = dataReader.ReadByte();
	atp.unknown13 = dataReader.ReadByte();

	atp.pbSensitivity = dataReader.ReadByte();

	atp.unknown182 = dataReader.ReadByte();
	atp.unknown181 = dataReader.ReadByte();

	atp.lowGainTimes2 = dataReader.ReadByte();
	atp.midGainTimes2 = dataReader.ReadByte();
	atp.highGainTimes2 = dataReader.ReadByte();

	atp.unknown18 = dataReader.ReadByte();
	ASSERT((atp.unknown18 == 0x61)
		   || (atp.unknown18 == 0x5a), "???");

	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown1811 = dataReader.ReadByte();
	atp.unknown1812 = dataReader.ReadByte();

	ASSERT_EQUALS(0xFF, dataReader.ReadByte());
	ASSERT_EQUALS(0xFFFFFFFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFFFFFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFFFFFF, dataReader.ReadUInt32());
	ASSERT_EQUALS(0xFFFF, dataReader.ReadUInt16());
	ASSERT_EQUALS(0xff, dataReader.ReadByte());

	atp.unknown19 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown401 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown24 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown402 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown23 = dataReader.ReadByte();
	atp.unknown25 = dataReader.ReadByte();
	atp.unknown27 = dataReader.ReadByte();
	atp.unknown28 = dataReader.ReadByte();
	atp.unknown26 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());

	atp.unknown239 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown20 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown301 = dataReader.ReadByte();
	atp.unknown22 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown226 = dataReader.ReadByte();
	atp.unknown218 = dataReader.ReadByte();
	atp.unknown221 = dataReader.ReadByte();
	atp.unknown220 = dataReader.ReadByte();
	atp.unknown217 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());

	atp.unknown224 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown211 = dataReader.ReadByte();

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

	atp.unknown236 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown212 = dataReader.ReadByte();
	atp.unknown238 = dataReader.ReadByte();
	atp.unknown213 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());

	atp.unknown231 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown21 = dataReader.ReadByte();

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

	atp.unknown241 = dataReader.ReadByte();
	atp.unknown214 = dataReader.ReadByte();
	atp.unknown242 = dataReader.ReadByte();
	atp.unknown234 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());

	atp.unknown235 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown216 = dataReader.ReadByte();

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

	atp.unknown232 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown233 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown219 = dataReader.ReadByte();

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

	atp.unknown222 = dataReader.ReadByte();
	atp.unknown225 = dataReader.ReadByte();
	atp.unknown223 = dataReader.ReadByte();
	atp.unknown307 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());

	atp.unknown306 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown215 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown304 = dataReader.ReadByte();
	atp.unknown237 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown305 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown310 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0x5a, dataReader.ReadByte());

	atp.unknown240 = dataReader.ReadByte();

	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	ASSERT_EQUALS(0x40, dataReader.ReadByte());

	atp.unknown31 = dataReader.ReadByte();

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
	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown302 = dataReader.ReadByte();
	atp.unknown303 = dataReader.ReadByte();

	ASSERT_EQUALS(0x5a, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
	ASSERT_EQUALS(0x40, dataReader.ReadByte());
}

void PerformanceReader::Read0x0F000008Chunk(DataReader &dataReader)
{
	uint32 perfIndex = dataReader.ReadUInt32();
	ASSERT(perfIndex <= 4, String::Number(perfIndex));
	perfIndex %= 4; //0-3 for style performances and 4 for performances

	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
	this->keyboardSettings[perfIndex].name = textReader.ReadZeroTerminatedString(41);
}

void PerformanceReader::Read0x10000008Chunk(DataReader &dataReader)
{
	auto& data = this->keyboardSettings[this->perfIndex]._0x_10000008_chunk;

	data.unknown21 = dataReader.ReadByte();
	ASSERT((data.unknown21 == 4)
		   || (data.unknown21 == 2)
		   || (data.unknown21 == 1)
		   , String::HexNumber(data.unknown21));

	data.unknown22 = dataReader.ReadByte();
	ASSERT((data.unknown22 == 2)
		   || (data.unknown22 == 1), "???");

	ASSERT_EQUALS(0, dataReader.ReadByte());

	data.unknown24 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	data.unknown23 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());
}

void PerformanceReader::Read0x12000108Chunk(DataReader &dataReader)
{
	auto& data = this->keyboardSettings[this->perfIndex]._0x12000108_chunk;

	data.unknown1 = dataReader.ReadByte();
	ASSERT_EQUALS(0xDE, dataReader.ReadByte());

	data.unknown21 = dataReader.ReadByte();
	data.unknown22 = dataReader.ReadByte();
	data.unknown221 = dataReader.ReadByte();
	data.unknown23 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	data.unknown25 = dataReader.ReadByte();

	ASSERT_EQUALS(1, dataReader.ReadByte());

	data.unknown24 = dataReader.ReadByte();

	ASSERT_EQUALS(2, dataReader.ReadByte());

	data.unknown26 = dataReader.ReadByte();
}
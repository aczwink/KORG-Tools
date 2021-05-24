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
#include "KeyboardSettingsV0_0Reader.hpp"
#include "../TrackPropertiesReader.hpp"
#include "../PadSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
libKORG::ChunkReader *KeyboardSettingsV0_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void KeyboardSettingsV0_0Reader::Read0x0FChunk(DataReader &dataReader)
{
	int32 perfIndex = dataReader.ReadInt32();
	ASSERT(perfIndex <= 4, String::Number(perfIndex));
	perfIndex %= 4; //0-3 for style performances and 4 for performances

	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
	this->keyboardSettings[perfIndex].name = textReader.ReadZeroTerminatedString(33);
	this->keyboardSettings[perfIndex].unknown1 = dataReader.ReadUInt32();
}

void KeyboardSettingsV0_0Reader::Read0x10000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x10000008_chunk;

	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadByte();
	chunk.unknown4 = dataReader.ReadByte();
	chunk.unknown5 = dataReader.ReadInt32();
	chunk.unknown6 = dataReader.ReadByte();
}

void KeyboardSettingsV0_0Reader::Read0x11000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x11000008_chunk;

	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
}

void KeyboardSettingsV0_0Reader::Read0x12000108Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x12000108_chunk;

	dataReader.ReadBytes(chunk.unknown1, sizeof(chunk.unknown1));
	dataReader.ReadBytes(chunk.unknown2, sizeof(chunk.unknown2));
	dataReader.ReadBytes(chunk.unknown3, sizeof(chunk.unknown3));
}

void KeyboardSettingsV0_0Reader::Read0x13000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x13000008_chunk;

	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
	dataReader.ReadBytes(chunk.unknown3, sizeof(chunk.unknown3));
}

void KeyboardSettingsV0_0Reader::Read0x14000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x14000008_chunk;

	dataReader.ReadBytes(chunk.unknown, sizeof(chunk.unknown));
}

void KeyboardSettingsV0_0Reader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x07000008:
			this->Read0x07000008Chunk(dataReader);
			break;
		case 0x08000008:
		{
			PadSettingsReader padSettingsReader(this->keyboardSettings[this->perfIndex].padSettings);
			padSettingsReader.Read(dataReader);
		}
		break;
		case 0x09000008:
		{
			TrackPropertiesReader trackPropertiesReader(this->keyboardSettings[this->perfIndex].trackProperties);
			trackPropertiesReader.Read(chunkHeader, dataReader);
		}
		break;
		case 0x0F000008:
			this->Read0x0FChunk(dataReader);
			break;
		case 0x10000008:
			this->Read0x10000008Chunk(dataReader);
			break;
		case 0x11000008:
			this->Read0x11000008Chunk(dataReader);
			break;
		case 0x12000108:
			this->Read0x12000108Chunk(dataReader);
			break;
		case 0x13000008:
			this->Read0x13000008Chunk(dataReader);
			break;
		case 0x14000008:
			this->Read0x14000008Chunk(dataReader);
			break;
		case 0x15000108:
		{
			int32 perfIndex = dataReader.ReadInt32();
			this->keyboardSettings[perfIndex]._0x15000108_chunk.unknown = dataReader.ReadInt32();
		}
		break;
	}
}

//Private methods
void KeyboardSettingsV0_0Reader::Read0x07000008Chunk(DataReader &dataReader)
{
	//TODO: convert
	int32 unknown1 = dataReader.ReadInt32();
	int8 unknown2 = dataReader.ReadInt8();
	int8 unknown3 = dataReader.ReadInt8();
	uint8 unknown4 = dataReader.ReadByte();
	uint8 unknown5 = dataReader.ReadByte();
	int8 unknown6 = dataReader.ReadInt8();
	uint8 unknown7 = dataReader.ReadByte();
	uint8 unknown8 = dataReader.ReadByte();
	uint8 unknown9 = dataReader.ReadByte();
	int16 unknown10 = dataReader.ReadInt16();

	uint8 unknown11[64];
	dataReader.ReadBytes(unknown11, sizeof(unknown11));
}

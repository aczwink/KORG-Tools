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
#include "KeyboardSettingsV1_0Reader.hpp"
#include "../TrackPropertiesReader.hpp"
#include "../MasterFXReader.hpp"
#include "../PadSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
libKORG::ChunkReader *KeyboardSettingsV1_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void KeyboardSettingsV1_0Reader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x08000008:
		{
			PadSettingsReader padSettingsReader(this->keyboardSettings[this->perfIndex].padSettings);
			padSettingsReader.Read(dataReader);
		}
		break;
		case 0x09010008:
		case 0x09010108:
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
		case 0x18000008:
			this->Read0x18000008Chunk(dataReader);
			break;
		case 0x1D000008:
		{
			MasterFXReader masterFxReader(this->keyboardSettings[this->perfIndex].masterFxSettings);
			masterFxReader.Read(dataReader);
		}
		break;
		case 0x22000008:
			this->Read0x22000008Chunk(dataReader);
			break;
	}
}

void KeyboardSettingsV1_0Reader::Read0x18000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x18000008_chunk;

	for(uint8 i = 0; i < 9; i++)
		chunk.unknown1[i] = dataReader.ReadUInt16();

	dataReader.ReadBytes(chunk.unknown2, sizeof(chunk.unknown2));
}

void KeyboardSettingsV1_0Reader::Read0x22000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->keyboardSettings[this->perfIndex]._0x22000008_chunk;

	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
}
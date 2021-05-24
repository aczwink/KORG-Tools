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
#include "AccompanimentSettingsV0_0Reader.hpp"
//Local
#include "../TrackPropertiesReader.hpp"
#include "../PadSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

libKORG::ChunkReader *AccompanimentSettingsV0_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void AccompanimentSettingsV0_0Reader::ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x06000008:
		{
			this->accompanimentSettings._0x06000008_chunk.unknown1 = dataReader.ReadInt8();
			this->accompanimentSettings._0x06000008_chunk.unknown2 = dataReader.ReadInt8();
		}
		break;
		case 0x07000008:
			this->Read0x07000008Chunk(dataReader);
		break;
		case 0x08000008:
		{
			PadSettingsReader padSettingsReader(this->accompanimentSettings.padSettings);
			padSettingsReader.Read(dataReader);
		}
		break;
		case 0x09000008:
		{
			TrackPropertiesReader trackPropertiesReader(this->accompanimentSettings.trackProperties);
			trackPropertiesReader.Read(chunkHeader, dataReader);
		}
		break;
	}
}

//Private methods
void AccompanimentSettingsV0_0Reader::Read0x07000008Chunk(DataReader &dataReader)
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

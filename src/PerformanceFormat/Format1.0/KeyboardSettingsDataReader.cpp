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
#include "KeyboardSettingsDataReader.hpp"
//Namespaces
using namespace libKORG::Performance::V1;
using namespace StdXX;

//Public methods
void KeyboardSettingsDataReader::Read0x0FChunk(String& name, uint32& unknown1, DataReader &dataReader)
{
	int32 perfIndex = dataReader.ReadInt32();
	ASSERT(perfIndex <= 4, String::Number(perfIndex));
	perfIndex %= 4; //0-3 for style performances and 4 for performances

	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
	name = textReader.ReadZeroTerminatedString(33);
	unknown1 = dataReader.ReadUInt32();
}

void KeyboardSettingsDataReader::Read0x10000008Chunk(_0x10000008_chunk& chunk, DataReader &dataReader)
{
	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadByte();
	chunk.unknown4 = dataReader.ReadByte();
	chunk.unknown5 = dataReader.ReadInt32();
	chunk.unknown6 = dataReader.ReadByte();
}

void KeyboardSettingsDataReader::Read0x11000008Chunk(_0x11000008_chunk& chunk, DataReader &dataReader)
{
	chunk.unknown1 = dataReader.ReadByte();
	chunk.unknown2 = dataReader.ReadByte();
}

void KeyboardSettingsDataReader::Read0x12000108Chunk(_0x12000108_chunk& chunk, DataReader &dataReader)
{
	dataReader.ReadBytes(chunk.unknown1, sizeof(chunk.unknown1));
	dataReader.ReadBytes(chunk.unknown2, sizeof(chunk.unknown2));
	dataReader.ReadBytes(chunk.unknown3, sizeof(chunk.unknown3));
}

void KeyboardSettingsDataReader::Read0x14000008Chunk(_0x14000008_chunk& chunk, DataReader &dataReader)
{
	dataReader.ReadBytes(chunk.unknown, sizeof(chunk.unknown));
}

void KeyboardSettingsDataReader::Read0x18000008Chunk(_0x18000008_chunk &chunk, DataReader &dataReader)
{
	for(uint8 i = 0; i < 9; i++)
		chunk.unknown1[i] = dataReader.ReadUInt16();

	dataReader.ReadBytes(chunk.unknown2, sizeof(chunk.unknown2));
}

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
#include "PerformanceFormat1_0VReader.hpp"
//Local
#include <libkorg/ProgramChangeSequence.hpp>
#include "../../Reading/PerformanceDataFormat.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
ChunkReader *PerformanceFormat1_0VReader::OnEnteringChunk(const ChunkHeader &chunkHeader)
{
	switch (chunkHeader.id)
	{
		case 0x01000000:
			return this;
		case 0x02000000:
			return &this->accompanimentSettingsReader;
		case 0x03000000:
			return &this->keyboardSettingsReader;
	}

	return nullptr;
}

void PerformanceFormat1_0VReader::OnLeavingChunk(const ChunkHeader &chunkHeader)
{
	if(chunkHeader.type == 3)
	{
		this->perfIndex++;
		this->keyboardSettingsReader.PerfIndex(this->perfIndex);
	}

	ChunkReader::OnLeavingChunk(chunkHeader);
}

void PerformanceFormat1_0VReader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x04000108:
			this->Read0x04000108Chunk(dataReader);
			break;
	}
}

//Private methods
void PerformanceFormat1_0VReader::Read0x04000108Chunk(DataReader &dataReader)
{
	//TODO: convert
	int32 unknown1 = dataReader.ReadInt32();
	uint8 unknown2 = dataReader.ReadByte();
	uint8 unknown3 = dataReader.ReadByte();

	uint8 unknown4[10];
	dataReader.ReadBytes(unknown4, sizeof(unknown4));

	uint8 unknown5[8];
	dataReader.ReadBytes(unknown5, sizeof(unknown5));

	uint8 unknown6[8];
	dataReader.ReadBytes(unknown6, sizeof(unknown6));

	uint8 unknown7[3];
	dataReader.ReadBytes(unknown7, sizeof(unknown7));

	uint8 unknown8[14*3];
	dataReader.ReadBytes(unknown8, sizeof(unknown8));
}

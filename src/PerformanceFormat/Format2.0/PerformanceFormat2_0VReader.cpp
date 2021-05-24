/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "PerformanceFormat2_0VReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
ChunkReader *PerformanceFormat2_0VReader::OnEnteringChunk(const ChunkHeader &chunkHeader)
{
	switch (chunkHeader.id)
	{
		case 0x1C000000:
			return &this->unknownAdditionalReader;
	}

	return PerformanceFormat1_0VReader::OnEnteringChunk(chunkHeader);
}

void PerformanceFormat2_0VReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x04020008:
			this->Read0x04020008Chunk(dataReader);
			break;
		case 0x20000008:
			this->Read0x20000008Chunk(dataReader);
			break;
	}
}

//Private methods
void PerformanceFormat2_0VReader::Read0x04020008Chunk(DataReader &dataReader)
{
	auto& chunk = this->generalPerformanceData._0x04020008;

	chunk.unknown1 = dataReader.ReadInt32();

	this->ReadUnknownSubChunk(chunk.subChunk, dataReader);

	dataReader.ReadBytes(chunk.unknown8, sizeof(chunk.unknown8));
	dataReader.ReadBytes(chunk.unknown9, sizeof(chunk.unknown9));
	dataReader.ReadBytes(chunk.unknown10, sizeof(chunk.unknown10));
	dataReader.ReadBytes(chunk.unknown11, sizeof(chunk.unknown11));
	dataReader.ReadBytes(chunk.unknown12, sizeof(chunk.unknown12));
}

void PerformanceFormat2_0VReader::Read0x20000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->generalPerformanceData._0x20000008;

	chunk.unknown1 = dataReader.ReadInt32();

	this->ReadUnknownSubChunk(chunk.subChunk, dataReader);

	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadUInt16();

	dataReader.ReadBytes(chunk.unknown4, sizeof(chunk.unknown4));
	dataReader.ReadBytes(chunk.unknown5, sizeof(chunk.unknown5));
	dataReader.ReadBytes(chunk.unknown6, sizeof(chunk.unknown6));
	dataReader.ReadBytes(chunk.unknown11, sizeof(chunk.unknown11));
	dataReader.ReadBytes(chunk.unknown12, sizeof(chunk.unknown12));
}

void PerformanceFormat2_0VReader::ReadUnknownSubChunk(Performance::UnknownSubChunk& chunk, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	chunk.unknown1 = dataReader.ReadByte();
	if(chunk.unknown1 != 0)
		return;

	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadByte();
	chunk.unknown4 = dataReader.ReadByte();
	chunk.unknown5 = textReader.ReadZeroTerminatedString();
	chunk.unknown6 = textReader.ReadZeroTerminatedString();

	uint32 unknownCount = dataReader.ReadUInt32();
	chunk.unknown7.Resize(unknownCount);
	for(uint32 i = 0; i < unknownCount; i++)
	{
		chunk.unknown7[i] = dataReader.ReadByte();
	}
}
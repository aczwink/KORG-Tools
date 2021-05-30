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

//Public methods
BankFormat::BankObject *PerformanceFormat1_0VReader::TakeResult()
{
	switch(this->perfIndex)
	{
		case 1:
			return new PerformanceObject(new Performance::V1::PerformanceData(Move(this->generalData), Move(this->keyboardSettings[0])));
		case 4:
			return new SingleTouchSettings(new Performance::V1::STSData(Move(this->generalData), Move(this->keyboardSettings)));
	}
	return nullptr;
}

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
		case 0x1C000000:
			return &this->unknownAdditionalReader;
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
	auto& chunk = this->generalData._0x04000108_data;

	int32 fifteen = dataReader.ReadInt32();
	ASSERT_EQUALS(0xF, fifteen);

	chunk.unknown2 = dataReader.ReadByte();
	chunk.unknown3 = dataReader.ReadByte();
	chunk.unknown4 = dataReader.ReadByte();
	chunk.metronomeTempo = dataReader.ReadByte();
	dataReader.ReadBytes(chunk.unknown41, sizeof(chunk.unknown41));
	dataReader.ReadBytes(chunk.unknown5, sizeof(chunk.unknown5));
	dataReader.ReadBytes(chunk.unknown6, sizeof(chunk.unknown6));
	dataReader.ReadBytes(chunk.unknown7, sizeof(chunk.unknown7));
	dataReader.ReadBytes(chunk.unknown8, sizeof(chunk.unknown8));
}
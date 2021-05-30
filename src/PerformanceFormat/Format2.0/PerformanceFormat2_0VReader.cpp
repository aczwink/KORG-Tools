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
//Local
#include <libkorg/BankFormat/PerformanceObject.hpp>
#include <libkorg/BankFormat/SingleTouchSettings.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
BankFormat::BankObject *PerformanceFormat2_0VReader::TakeResult()
{
	switch(this->perfIndex)
	{
		case 1:
			return new PerformanceObject(new Performance::V2::PerformanceData(Move(this->generalData), Move(this->keyboardSettings[0])));
		case 4:
			return new SingleTouchSettings(new Performance::V2::STSData(Move(this->generalData), Move(this->keyboardSettings)));
	}
	return nullptr;
}

//Protected methods
ChunkReader *PerformanceFormat2_0VReader::OnEnteringChunk(const ChunkHeader &chunkHeader)
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

void PerformanceFormat2_0VReader::OnLeavingChunk(const ChunkHeader &chunkHeader)
{
	if(chunkHeader.type == 3)
	{
		this->perfIndex++;
		this->keyboardSettingsReader.PerfIndex(this->perfIndex);
	}

	ChunkReader::OnLeavingChunk(chunkHeader);
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
	auto& chunk = this->generalData._0x04020008_data;

	chunk.unknown1 = dataReader.ReadInt32();

	this->ReadUnknownSubChunk(chunk.subChunk, dataReader);

	chunk.unknown81 = dataReader.ReadByte();

	chunk.metronomeTempo = dataReader.ReadByte();

	dataReader.ReadBytes(chunk.unknown83, sizeof(chunk.unknown83));
	dataReader.ReadBytes(chunk.unknown9, sizeof(chunk.unknown9));
	dataReader.ReadBytes(chunk.unknown10, sizeof(chunk.unknown10));
	dataReader.ReadBytes(chunk.unknown11, sizeof(chunk.unknown11));
	dataReader.ReadBytes(chunk.unknown12, sizeof(chunk.unknown12));
}

void PerformanceFormat2_0VReader::Read0x20000008Chunk(DataReader &dataReader)
{
	auto& chunk = this->generalData._0x20000008_data;

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

void PerformanceFormat2_0VReader::ReadUnknownSubChunk(Performance::V2::UnknownSubChunk& chunk, DataReader &dataReader)
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
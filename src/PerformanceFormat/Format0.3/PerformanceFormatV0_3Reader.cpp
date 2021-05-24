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
#include "PerformanceFormatV0_3Reader.hpp"
//Namespaces
using namespace StdXX;

//Public methods
libKORG::BankFormat::BankObject *PerformanceFormatV0_3Reader::TakeResult()
{
	if(this->stylePerformances)
		return this->TakeSTSResult();
	return this->TakePerformanceResult();
}

//Protected methods
libKORG::ChunkReader *PerformanceFormatV0_3Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void PerformanceFormatV0_3Reader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x00010008:
			this->Read0x00010008Chunk(dataReader);
			break;
		case 0x02000108:
			this->Read0x02000108Chunk(dataReader);
			break;
	}
}

//Private methods
void PerformanceFormatV0_3Reader::Read0x00010008Chunk(DataReader &dataReader)
{
	//TODO: convert
	dataReader.ReadUInt32();
	dataReader.ReadUInt32();
	dataReader.ReadUInt32();

	uint8 unknownCount = dataReader.ReadByte();
	dataReader.ReadByte();




	uint8 unknown1[9];
	dataReader.ReadBytes(unknown1, sizeof(unknown1));

	uint8 unknown2[8];
	dataReader.ReadBytes(unknown2, sizeof(unknown2));

	uint8 unknown3[8];
	dataReader.ReadBytes(unknown3, sizeof(unknown3));

	uint8 unknown4[10];
	dataReader.ReadBytes(unknown4, sizeof(unknown4));

	uint8 unknown5[10];
	dataReader.ReadBytes(unknown5, sizeof(unknown5));

	uint8 unknown6[10];
	dataReader.ReadBytes(unknown6, sizeof(unknown6));





	int8 unknown7 = dataReader.ReadInt8();

	this->ReadEffectGroup(dataReader);

	for(uint8 i = 0; i < 8; i++)
	{
		this->ReadTrackParams(dataReader);
	}





	for(uint8 i = 0; i < unknownCount; i++)
	{
		uint8 unknown31 = dataReader.ReadByte();
		uint8 unknown32 = dataReader.ReadByte();
		uint8 unknown33 = dataReader.ReadByte();





		uint8 unknown34 = dataReader.ReadByte();
		uint8 unknown35 = dataReader.ReadByte();





		uint8 unknown36 = dataReader.ReadByte();
		uint8 unknown37 = dataReader.ReadByte();
		uint8 unknown38 = dataReader.ReadByte();
		int8 unknown39 = dataReader.ReadInt8();
		int8 unknown40 = dataReader.ReadInt8();
		uint8 unknown41 = dataReader.ReadByte();

		for(uint8 j = 0; j < 3; j++)
		{
			uint8 unknown42 = dataReader.ReadByte();
			uint8 unknown43 = dataReader.ReadByte();
		}





		for(uint8 j = 0; j < 4; j++)
		{
			uint8 unknown51 = dataReader.ReadUInt16();
			uint8 unknown52 = dataReader.ReadByte();

			uint8 unknown53 = dataReader.ReadByte();
			uint8 unknown54 = dataReader.ReadByte();
			uint8 unknown55 = dataReader.ReadByte();
			uint8 unknown56 = dataReader.ReadByte();
			uint8 unknown57 = dataReader.ReadByte();
			uint8 unknown58 = dataReader.ReadByte();
			uint8 unknown59 = dataReader.ReadByte();
		}





		uint8 unknown61 = dataReader.ReadByte();
		uint8 unknown62 = dataReader.ReadByte();
		for(uint8 j = 0; j < 12; j++)
		{
			uint8 unknown63 = dataReader.ReadByte();
		}

		this->ReadEffectGroup(dataReader);






		for(uint8 j = 0; j < 4; j++)
		{
			this->ReadTrackParams(dataReader);
		}
	}

	uint32 unknown99 = dataReader.ReadUInt32();
}

void PerformanceFormatV0_3Reader::Read0x02000108Chunk(DataReader &dataReader)
{
	//TODO: convert
	uint32 unknown01 = dataReader.ReadUInt32();
	uint32 unknown02 = dataReader.ReadUInt32();
	uint32 unknown03 = dataReader.ReadUInt32();
	uint32 unknown04 = dataReader.ReadUInt32();
	uint8 nUnknown1 = dataReader.ReadByte();
	uint8 unknown06 = dataReader.ReadByte();





	uint8 unknown10 = dataReader.ReadByte();
	for(uint8 i = 0; i < 2; i++)
	{
		uint8 unknown11 = dataReader.ReadByte();
		uint8 unknown111 = dataReader.ReadByte();
		uint8 unknown112 = dataReader.ReadByte();
	}

	int32 unknown12 = dataReader.ReadInt32();







	int32 unknown20 = dataReader.ReadInt32();
	int32 unknown21 = dataReader.ReadInt32();
	uint32 unknown22 = dataReader.ReadUInt32();
	uint32 unknown23 = dataReader.ReadUInt32();
	uint32 unknown24 = dataReader.ReadUInt32();
	int32 unknown25 = dataReader.ReadInt32();
	int32 unknown26 = dataReader.ReadInt32();
	int32 unknown27 = dataReader.ReadInt32();
	int32 unknown28 = dataReader.ReadInt32();







	for(uint8 i = 0; i < 8; i++)
	{
		uint32 unknown30 = dataReader.ReadUInt32();

		uint8 unknown31[14];
		dataReader.ReadBytes(unknown31, sizeof(unknown31));
	}

	for(uint8 i = 0; i < 4; i++)
	{
		uint16 unknown40 = dataReader.ReadUInt16();

		uint8 unknown41[8];
		dataReader.ReadBytes(unknown41, sizeof(unknown41));
	}







	for(uint8 i = 0; i < nUnknown1; i++)
	{
		uint8 unknown50 = dataReader.ReadByte();

		int8 unknown51[33];
		dataReader.ReadBytes(unknown51, sizeof(unknown51));

		int32 unknown52 = dataReader.ReadInt32();






		int32 unknown53 = dataReader.ReadInt32();
		uint8 unknown54 = dataReader.ReadByte();
		uint32 unknown55 = dataReader.ReadUInt32();







		int32 unknown56 = dataReader.ReadInt32();
		uint8 unknown57 = dataReader.ReadByte();
		uint8 unknown58 = dataReader.ReadByte();
		uint8 unknown59 = dataReader.ReadByte();
		uint8 unknown60 = dataReader.ReadByte();







		for(uint8 j = 0; j < 4; j++)
		{
			uint16 unknown70 = dataReader.ReadUInt16();

			uint8 unknown71[8];
			dataReader.ReadBytes(unknown71, sizeof(unknown71));
		}







		for(uint8 j = 0; j < 4; j++)
		{
			uint32 unknown80 = dataReader.ReadUInt32();

			uint8 unknown81[14];
			dataReader.ReadBytes(unknown81, sizeof(unknown81));
		}
	}

	dataReader.ReadUInt32();
}

void PerformanceFormatV0_3Reader::ReadEffectGroup(DataReader &dataReader)
{
	//TODO: convert
	for(uint8 i = 0; i < 2; i++)
	{
		int16 unknown8 = dataReader.ReadInt16();
		uint8 unknown9[64];
		dataReader.ReadBytes(unknown9, sizeof(unknown9));
	}

	int16 unknown10 = dataReader.ReadInt16();
	uint8 unknown11 = dataReader.ReadByte();
	dataReader.ReadByte();
	dataReader.ReadByte();
	dataReader.ReadByte();
}

void PerformanceFormatV0_3Reader::ReadTrackParams(DataReader &dataReader)
{
	//TODO: convert
	uint32 unknown12 = dataReader.ReadUInt32();
	uint8 unknown13 = dataReader.ReadByte();
	uint8 unknown14 = dataReader.ReadByte();
	uint8 unknown15 = dataReader.ReadByte();
	uint8 unknown16 = dataReader.ReadByte();
	uint32 unknown17 = dataReader.ReadUInt32();
	uint8 unknown18 = dataReader.ReadByte();
	int8 unknown19 = dataReader.ReadInt8();
	uint8 unknown20 = dataReader.ReadByte();
	int8 unknown21 = dataReader.ReadInt8();

	uint8 unknown22[16];
	dataReader.ReadBytes(unknown22, sizeof(unknown22));

	uint32 unknown23 = dataReader.ReadUInt32();
	uint8 unknown24 = dataReader.ReadByte();
}
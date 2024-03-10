/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
//Corresponding header
#include "OC31.hpp"
//Namespaces
using namespace StdXX;

//Namespace functions
uint32 libKORG::OC31ComputeOptimalBlockSize(const OC31BlockHeader &block)
{
	uint8 headerLen;
	if(block.isBackreference)
	{
		if((block.length <= 8) && (block.distance <= 0x7FF))
			headerLen = 2;
		else if(block.distance < 0x4000)
		{
			if(block.length <= (0x1F + 2))
				headerLen = 1;
			else if( (block.length >= 0x22) and (block.length <= (Unsigned<uint8>::Max() + 0x22)) )
				headerLen = 2;
			else
				headerLen = 3;

			headerLen += 2;
		}
		else
		{
			if(block.length <= (7+2))
				headerLen = 1;
			else if( (block.length >= 0xA) and (block.length <= (Unsigned<uint8>::Max() + 0xA)) )
				headerLen = 2;
			else
				headerLen = 3;

			headerLen += 2;
		}

		return headerLen + block.nExtraBytes;
	}

	if( (block.length >= 4) and (block.length <= 17) )
		headerLen = 1;
	else if( (block.length >= 0x12) and (block.length <= (0x12 + Unsigned<uint8>::Max())) )
		headerLen = 2;
	else
		headerLen = 3;
	return headerLen + block.length;
}

DynamicArray<libKORG::OC31Block> libKORG::OC31ReadAllBlocks(StdXX::InputStream &inputStream)
{
	DynamicArray<libKORG::OC31Block> blocks;
	DataReader leReader(false, inputStream);
	DataReader dataReader(true, inputStream);

	uint32 signature = leReader.ReadUInt32();
	if(signature != FOURCC(u8"OC31"))
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	uint32 uncompressedSize = leReader.ReadUInt32();

	uint32 leftSize = uncompressedSize;
	while(leftSize)
	{
		libKORG::OC31Block block;
		OC31ReadBlockHeader(dataReader, block);

		if(block.isBackreference)
		{
			dataReader.ReadBytes(block.extra, block.nExtraBytes);

			leftSize -= block.length;
			leftSize -= block.nExtraBytes;
		}
		else
		{
			block.rawData = new FixedSizeBuffer(block.length);
			dataReader.ReadBytes(block.rawData->Data(), block.length);

			leftSize -= block.length;
		}

		blocks.Push(Move(block));
	}

	uint8 checkValue = dataReader.ReadByte();

	return blocks;
}

void libKORG::OC31ReadBlockHeader(DataReader &dataReader, OC31BlockHeader &header)
{
	uint8 flagByte = dataReader.ReadByte();

	if(flagByte & 0xF0)
	{
		header.isBackreference = true;

		uint8 nExtraBytes;
		uint16 offset, length;

		if(flagByte & 0xC0u)
		{
			uint8 packed = dataReader.ReadByte();

			length = (flagByte >> 5u) + 1;
			offset = (uint16(flagByte & 0x1F) << 6) | (packed >> 2);
			nExtraBytes = packed & 3;
		}
		else if(flagByte & 0x20u)
		{
			uint8 indicator = flagByte & 0x1Fu;
			if(indicator == 0)
				length = dataReader.ReadByte() + 0x22_u16;
			else if(indicator == 1)
				length = dataReader.ReadUInt16();
			else
				length = indicator + 2_u16;

			uint16 tmp = dataReader.ReadUInt16();
			offset = tmp >> 2;
			nExtraBytes = tmp & 3;
		}
		else
		{
			uint8 indicator = flagByte & 7;
			if(indicator == 0)
				length = dataReader.ReadByte() + 0xA_u16;
			else if(indicator == 1)
				length = dataReader.ReadUInt16();
			else
				length = indicator + 2_u16;

			uint16 tmp = dataReader.ReadUInt16();
			offset = tmp >> 2;
			offset += 0x4000;
			nExtraBytes = tmp & 3;

			if(flagByte & 8)
				offset += 0x4000;
		}

		header.length = length;
		header.nExtraBytes = nExtraBytes;
		header.distance = offset;
	}
	else
	{
		header.isBackreference = false;

		if(flagByte == 0)
			header.length = dataReader.ReadByte() + 0x12_u16;
		else if(flagByte == 1)
			header.length = dataReader.ReadUInt16();
		else
			header.length = flagByte + 2_u16;
	}
}
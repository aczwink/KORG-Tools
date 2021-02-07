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
#include "ZLibBlocksInputStream.hpp"
//Namespaces
using namespace StdXX;

/*
 * The block structure and in particular block 0x100 are based on the info provided by Mateusz Faderewski (https://github.com/Polprzewodnikowy/KorgPackage)
 */

//Private methods
uint32 ZLibBlocksInputStream::ReadEndOfStreamBlock(DataReader& dataReader, uint64 from, uint64 to)
{
	ASSERT_EQUALS(this->fileSize, this->readFileSize);
	ASSERT(Math::IsValueInInterval(this->dataSize - this->readDataSize, from, to), String::Number(this->dataSize - this->readDataSize));

	while(this->readDataSize < this->dataSize)
	{
		ASSERT_EQUALS(0, dataReader.ReadByte())
		this->readDataSize++;
	}
	this->endBlockRead = true;
	return 0;
}

uint32 ZLibBlocksInputStream::ReadNextBlock(InputStream& baseInputStream, void* destination)
{
	if(this->endBlockRead)
		return 0;

	DataReader dataReader(false, baseInputStream);

	uint32 blockType = dataReader.ReadUInt32();
	this->readDataSize += 4;
	switch(blockType)
	{
		case 0x100:
			return this->ReadZLibBlock(dataReader, destination);

		//end of stream block
		case 0x101:
			return this->ReadEndOfStreamBlock(dataReader, 4, 4);

		default:
			stdErr << u8"Unknown block: " << String::HexNumber(blockType)
				   << u8" at: " << this->readDataSize << u8" of " << this->dataSize
				   << u8" / " << this->readFileSize << u8" of " << this->fileSize
				   << endl;
			NOT_IMPLEMENTED_ERROR;
	}
}

uint32 ZLibBlocksInputStream::ReadZLibBlock(DataReader &dataReader, void *destination)
{
	DataReader beReader(true, dataReader.InputStream());

	uint32 compressedSize = dataReader.ReadUInt32();
	uint32 uncompressedSize = beReader.ReadUInt32();

	ASSERT(uncompressedSize <= 2 * MiB, u8"Report this please!");

	compressedSize -= 4; //subtract the uncompressed size field
	LimitedInputStream limitedInputStream(dataReader.InputStream(), compressedSize);
	UniquePointer<Decompressor> decompressor = Decompressor::Create(CompressionStreamFormatType::zlib, limitedInputStream, this->verify);
	BufferOutputStream bufferOutputStream(destination, uncompressedSize);

	decompressor->FlushTo(bufferOutputStream);
	ASSERT_EQUALS(uncompressedSize, bufferOutputStream.GetCurrentOffset());

	this->readDataSize += 8 + compressedSize;
	this->readFileSize += uncompressedSize;

	//padding
	uint32 remainder = compressedSize % 4;
	if (remainder != 0)
	{
		for(uint32 i = 0; i < 4 - remainder; i++)
		{
			ASSERT_EQUALS(0, dataReader.ReadByte());
			this->readDataSize++;
		}
	}

	return uncompressedSize;
}

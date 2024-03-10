/*
 * Copyright (c) 2020-2024 Amir Czwink (amir130@hotmail.de)
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
#include "OC31Decompressor.hpp"
//Local functions
#include "OC31.hpp"
//Namespaces
using namespace StdXX;
using namespace libKORG;

const bool c_oc31endianness = true;

//Constructor
OC31Decompressor::OC31Decompressor(InputStream &inputStream) : Decompressor(inputStream),
	dictionary(Unsigned<uint16>::Max() << 1), dataReader(c_oc31endianness, inputStream)
{
	this->computedCheck = 0;
	this->nBytesInBuffer = 0;

	DataReader dataReader(false, inputStream);
	this->uncompressedSize = dataReader.ReadUInt32();
	this->leftUncompressedSize = this->uncompressedSize;
}

//Public methods
uint32 OC31Decompressor::GetBytesAvailable() const
{
	return this->nBytesInBuffer + this->inputStream.GetBytesAvailable();
}

bool OC31Decompressor::IsAtEnd() const
{
	return (this->nBytesInBuffer == 0) && this->inputStream.IsAtEnd();
}

uint32 OC31Decompressor::ReadBytes(void *destination, uint32 count)
{
	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		if(this->IsAtEnd())
			break;
		this->FillDictionaryIfEmpty();

		uint32 nBytesToCopy = Math::Min(count, (uint32)this->nBytesInBuffer);
		this->dictionary.Read(dest, this->nBytesInBuffer, nBytesToCopy);

		count -= nBytesToCopy;
		this->nBytesInBuffer -= nBytesToCopy;
		dest += nBytesToCopy;
	}
	this->FillDictionaryIfEmpty(); //make sure the end is read because of the check value

	return dest - static_cast<uint8 *>(destination);
}

//Private methods
void OC31Decompressor::Backreference(uint16 offset, uint16 length, uint8 nExtraBytes)
{
	this->dictionary.CopyToTail(offset + 1, length);

	while(nExtraBytes)
	{
		uint8 b = this->dataReader.ReadByte();
		this->dictionary.Append(b);
		nExtraBytes--;
	}
}

void OC31Decompressor::DecompressDirect(uint16 length)
{
	uint16 leftLength = length;
	while(leftLength)
	{
		byte buffer[4096];
		uint32 nBytesRead = this->inputStream.ReadBytes(buffer, leftLength);
		this->dictionary.Append(buffer, nBytesRead);

		leftLength -= nBytesRead;
	}
}

void OC31Decompressor::DecompressNextBlock()
{
	OC31BlockHeader chunkHeader;
	OC31ReadBlockHeader(this->dataReader, chunkHeader);

	uint16 length;
	if(chunkHeader.isBackreference)
	{
		this->Backreference(chunkHeader.distance, chunkHeader.length, chunkHeader.nExtraBytes);
		length = chunkHeader.length + chunkHeader.nExtraBytes;
	}
	else
	{
		this->DecompressDirect(chunkHeader.length);
		length = chunkHeader.length;
	}

	ASSERT(this->leftUncompressedSize >= length, u8"TODO: HANDLE THIS CORRECTLY");
	this->leftUncompressedSize -= length;

	for(uint16 i = 0; i < length; i++)
	{
		byte b;
		this->dictionary.Read(&b, i+1, 1);
		this->computedCheck ^= b;
	}
	this->nBytesInBuffer += length;

	if((this->nBytesInBuffer == 0) && (this->leftUncompressedSize == 0))
	{
		uint8 end[2];
		uint32 nBytesRead = this->inputStream.ReadBytes(end, 2);
		ASSERT_EQUALS(1, nBytesRead);
		ASSERT_EQUALS(end[0], this->computedCheck);
	}
}
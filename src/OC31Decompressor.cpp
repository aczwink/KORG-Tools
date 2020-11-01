/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
//Namespaces
using namespace StdXX;
using namespace libKORG;

const bool c_oc31endianness = true;

//Constructor
OC31Decompressor::OC31Decompressor(InputStream &inputStream) : Decompressor(inputStream),
	buffer(Unsigned<uint16>::Max()), dictionary(Unsigned<uint16>::Max()), dataReader(c_oc31endianness, inputStream)
{
	this->nBytesInBuffer = 0;

	DataReader dataReader(false, inputStream);
	this->uncompressedSize = dataReader.ReadUInt32();
}

//Public methods
uint32 OC31Decompressor::GetBytesAvailable() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

bool OC31Decompressor::IsAtEnd() const
{
	return this->inputStream.IsAtEnd() && (this->nBytesInBuffer == 0);
}

uint32 OC31Decompressor::ReadBytes(void *destination, uint32 count)
{
	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		if(this->IsAtEnd())
			break;
		if ((this->nBytesInBuffer == 0) && !this->inputStream.IsAtEnd())
			this->DecompressNextBlock();

		uint32 nBytesToCopy = Math::Min(count, (uint32)this->nBytesInBuffer);
		MemCopy(dest, this->buffer.Data(), nBytesToCopy);

		count -= nBytesToCopy;
		this->nBytesInBuffer -= nBytesToCopy;
		dest += nBytesToCopy;
	}

	return dest - static_cast<uint8 *>(destination);
}

uint32 OC31Decompressor::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

//Private methods
void OC31Decompressor::Backreference(uint16 offset, uint8 length, uint8 nExtraBytes)
{
	BufferOutputStream bufferOutputStream(this->buffer.Data(), this->buffer.Size());
	this->dictionary.Copy(offset + 1, length, bufferOutputStream);
	this->nBytesInBuffer = length;

	while(nExtraBytes)
	{
		uint8 b = this->dataReader.ReadByte();
		this->buffer.Data()[this->nBytesInBuffer++] = b;
		this->dictionary.Append(b);
		nExtraBytes--;
	}
}

void OC31Decompressor::DecompressBackreference(uint8 flagByte)
{
	if(flagByte & 0xC0u)
	{
		uint8 packed = this->dataReader.ReadByte();

		uint8 length = (flagByte >> 5u) + 1;
		uint16 offset = (uint16(flagByte & 0x1F) << 6) | (packed >> 2);
		uint8 nExtraBytes = packed & 3;

		this->Backreference(offset, length, nExtraBytes);
	}
	else if(flagByte & 0x20u)
	{
		uint8 indicator = flagByte & 0x1Fu;
		if(indicator == 0)
		{
			uint8 length = this->dataReader.ReadByte() + 0x22;
			uint16 tmp = this->dataReader.ReadUInt16();
			uint16 offset = tmp >> 2;
			uint8 nExtraBytes = tmp & 3;

			this->Backreference(offset, length, nExtraBytes);
		}
		else if(indicator == 1)
		{
			NOT_IMPLEMENTED_ERROR;
		}
		else
		{
			uint8 length = flagByte + 2;
			uint16 tmp = this->dataReader.ReadUInt16();
			uint16 offset = tmp >> 2;
			uint8 nExtraBytes = tmp & 3;

			this->Backreference(offset, length, nExtraBytes);
		}
	}
	else
	{
		NOT_IMPLEMENTED_ERROR;
	}
}

void OC31Decompressor::DecompressDirect(uint8 flagByte)
{
	uint16 length;
	if(flagByte == 0)
	{
		NOT_IMPLEMENTED_ERROR;
	}
	else if(flagByte == 1)
		length = this->dataReader.ReadUInt16() >> 2u;
	else
		length = flagByte + 2_u16;

	uint32 nBytesRead = this->inputStream.ReadBytes(this->buffer.Data(), length);
	ASSERT_EQUALS((uint32)length, nBytesRead);

	this->dictionary.Append(this->buffer.Data(), nBytesRead);
	this->nBytesInBuffer = length;
}

void OC31Decompressor::DecompressNextBlock()
{
	uint8 flagByte = this->dataReader.ReadByte();
	if(flagByte & 0xF0)
		this->DecompressBackreference(flagByte);
	else
		this->DecompressDirect(flagByte);
}
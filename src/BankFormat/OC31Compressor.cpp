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
#include "OC31Compressor.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Constructor
OC31Compressor::OC31Compressor(UniquePointer<SeekableOutputStream> &&baseStream) : Compressor(*baseStream), baseStream(Move(baseStream)), dictionary(Unsigned<uint16>::Max())
{
	this->checkValue = 0;
	this->uncompressedSize = 0;

	StdXX::TextWriter fourccWriter(*this->baseStream, StdXX::TextCodecType::ASCII);
	fourccWriter.WriteString(u8"OC31");

	this->uncompressedSizeOffset = this->baseStream->QueryCurrentOffset();
	this->baseStream->WriteInt32LE(0); //reserve space for the uncompressed size
}

//Public methods
void OC31Compressor::Finalize()
{
	Compressor::Finalize();

	DataWriter dataWriter(false, *this->baseStream);

	//write last block
	this->WriteByte(1);
	dataWriter.WriteUInt16(0);

	//write check value
	dataWriter.WriteByte(this->checkValue);

	//write uncompressed size
	uint64 offset = this->baseStream->QueryCurrentOffset();
	this->baseStream->SeekTo(this->uncompressedSizeOffset);
	dataWriter.WriteUInt32(this->uncompressedSize);
	this->baseStream->SeekTo(offset);
}

void OC31Compressor::Flush()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 OC31Compressor::WriteBytes(const void *source, uint32 size)
{
	const uint8* src = static_cast<const uint8 *>(source);
	while(size)
	{
		uint16 blockLength = (uint16)Math::Min((uint32)Unsigned<uint16>::Max(), size);
		for(uint16 i = 0; i < blockLength; i++)
		{
			this->dictionary.Append(*src);
			this->checkValue = this->checkValue xor (*src++);
		}

		auto match = this->dictionary.FindLongestMatchWithTail(blockLength);
		if(match.length < blockLength)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			//TODO: encode match and shrink blockLength by match size
			//this->WriteUncompressedBlock(src, blockLength);
		}
		else
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			//TODO: wait for more data
		}

		size -= blockLength;
	}

	uint32 totalWrittenBytes = src - static_cast<const uint8 *>(source);
	this->uncompressedSize += totalWrittenBytes;
	return totalWrittenBytes;
}

//Private methods
void OC31Compressor::WriteUncompressedBlock(const uint8 *data, uint16 length)
{
	if(length <= 17)
	{
		this->WriteByte(length - 2);
		this->baseStream->WriteBytes(data, length);
	}
	else if(length <= (0x12 + Unsigned<uint8>::Max()))
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	else
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}
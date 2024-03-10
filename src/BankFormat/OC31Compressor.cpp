/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "OC31.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

const uint16 oc31_maxDistance = 0xBFFF;

//Constructor
OC31Compressor::OC31Compressor(UniquePointer<SeekableOutputStream> &&baseStream) : Compressor(*baseStream), baseStream(Move(baseStream)), dictionary(Unsigned<uint16>::Max() << 1, 3, oc31_maxDistance)
{
	this->checkValue = 0;
	this->uncompressedSize = 0;
	this->nUnprocessedBytesInDictionary = 0;
	this->nQueuedBlocks = 0;

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

	while(this->nUnprocessedBytesInDictionary)
		this->EmitBlock();
	if(this->nQueuedBlocks == 2)
		this->FlushTopMostBlock();
	if(this->nQueuedBlocks)
		this->EncodeBlock(this->queuedBlocks[0], 0);

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
		uint16 space = oc31_maxDistance - this->nUnprocessedBytesInDictionary;
		uint16 nBytesToWrite = (uint16)Math::Min((uint32)space, size);
		for(uint16 i = 0; i < nBytesToWrite; i++)
		{
			this->dictionary.Append(*src);
			this->checkValue = this->checkValue xor (*src++);
		}

		this->nUnprocessedBytesInDictionary += nBytesToWrite;
		size -= nBytesToWrite;

		if(this->nUnprocessedBytesInDictionary == oc31_maxDistance)
			this->EmitBlock();
	}

	uint32 totalWrittenBytes = src - static_cast<const uint8 *>(source);
	this->uncompressedSize += totalWrittenBytes;
	return totalWrittenBytes;
}

//Private methods
uint32 OC31Compressor::ComputeRequiredBytesForBackreference(uint16 distance, uint16 length) const
{
	OC31BlockHeader header;
	header.isBackreference = true;
	header.distance = distance;
	header.length = length;
	header.nExtraBytes = 0;

	return OC31ComputeOptimalBlockSize(header);
}

uint8 OC31Compressor::ComputeRequiredBytesForStoringRawBytes(uint8 bytesCount) const
{
	if(this->nQueuedBlocks > 0)
	{
		const auto& queuedBlock = this->queuedBlocks[this->nQueuedBlocks - 1];
		if(queuedBlock.isBackreference and ( (queuedBlock.nExtraBytes + bytesCount) <= 3 ) )
			return bytesCount;
		else if(!queuedBlock.isBackreference)
			return bytesCount;
	}
	return bytesCount + 1_u8;
}

void OC31Compressor::EmitBlock()
{
	auto match = this->dictionary.FindLongestMatchAtSplitDistance(this->nUnprocessedBytesInDictionary);

	QueuedBlock nextBlock{
		.isBackreference = match.length >= 3, //the encoding requires a backref to have a min length of 3, some can only be encoded with 4
		.distance = match.distance,
		.length = match.length,
		.nExtraBytes = 0,
	};

	if(nextBlock.isBackreference and (nextBlock.length == 3) and (nextBlock.distance > 0x7FF))
	{
		nextBlock.isBackreference = false;
		nextBlock.length = 1; //to allow searching backreferences at the next byte
	}
	else if(!nextBlock.isBackreference)
		nextBlock.length = Math::Max(1_u16, nextBlock.length);

	this->FindOptimalBlock(nextBlock);
}

void OC31Compressor::EncodeBlock(const QueuedBlock &block, uint16 dataDistance)
{
	if(block.isBackreference)
	{
		uint8 buffer[3];
		this->dictionary.Read(buffer, dataDistance + block.nExtraBytes, block.nExtraBytes);
		this->WriteBackreference(block.distance - 1, block.length, block.nExtraBytes, buffer);
	}
	else
	{
		uint8 buffer[StdXX::Unsigned<uint16>::Max()];
		this->dictionary.Read(buffer, dataDistance + block.length, block.length);
		this->WriteUncompressedBlock(buffer, block.length);
	}
}

void OC31Compressor::FindOptimalBlock(const QueuedBlock& nextBlock)
{
	if(nextBlock.isBackreference)
	{
		uint32 backref1Count = this->ComputeRequiredBytesForBackreference(nextBlock.distance, nextBlock.length);

		//try to find a longer match within that backreference. Only need to search to 5 (max length of a backreference) because beyond that a backreference always pays out
		for(uint16 i = 1; i < Math::Min(nextBlock.length, 6_u16); i++)
		{
			auto match = this->dictionary.FindLongestMatchAtSplitDistance(this->nUnprocessedBytesInDictionary - i);

			//if((match.length - i) > nextBlock.length)
			if((match.length) > nextBlock.length)
			{
				//compute the "rest" of the matched longer reference, which can either be stored as another backreference or as extra bytes
				uint16 notTakenLength = match.length - (nextBlock.length - i);
				// well for total optimization it actually could matter how we choose them, since you could balance the two overlapping backreferences
				//to achieve minimal encoding lengths, but this is not implemented here

				uint32 notTakenRestEncoding = (notTakenLength >= 3) ? this->ComputeRequiredBytesForBackreference(match.distance, notTakenLength) : notTakenLength;
				uint32 byteSizeIfMatchNotTaken = backref1Count + notTakenRestEncoding; //size to store the original backreference with the rest of the remaining one

				uint8 extraByteCount = this->ComputeRequiredBytesForStoringRawBytes(i); //bytes for skipping over until "i"
				uint32 backref2Count = this->ComputeRequiredBytesForBackreference(match.distance, match.length); //bytes to encode the longer backreference
				uint32 byteSizeIfMatchTaken = extraByteCount + backref2Count;

				if(byteSizeIfMatchNotTaken <= byteSizeIfMatchTaken)
					continue;

				//its better to shift some bytes in this case and take the longer backreference (in next call)
				QueuedBlock rawBlock;
				rawBlock.length = i;
				rawBlock.isBackreference = false;

				this->PutBlockInQueue(rawBlock);
				this->nUnprocessedBytesInDictionary -= rawBlock.length;
				this->dictionary.IndexUpTo(this->nUnprocessedBytesInDictionary);

				return;
			}
		}
	}

	this->PutBlockInQueue(nextBlock);
	this->nUnprocessedBytesInDictionary -= nextBlock.length;
	this->dictionary.IndexUpTo(this->nUnprocessedBytesInDictionary);
}

void OC31Compressor::FlushTopMostBlock()
{
	if(this->queuedBlocks[0].isBackreference and !this->queuedBlocks[1].isBackreference)
	{
		if(this->queuedBlocks[1].length <= 3)
		{
			this->queuedBlocks[0].nExtraBytes = this->queuedBlocks[1].length;
			this->EncodeBlock(this->queuedBlocks[0], this->nUnprocessedBytesInDictionary);
			this->nQueuedBlocks = 0;

			return;
		}
		else if(
				Math::IsValueInInterval((int32)this->queuedBlocks[1].length, 17, 17 + 3)
				or
				Math::IsValueInInterval((int32)this->queuedBlocks[1].length, 0x12 + Unsigned<uint8>::Max(), 0x12 + Unsigned<uint8>::Max() + 3)
				)
		{
			this->queuedBlocks[0].nExtraBytes = 3;
			this->queuedBlocks[1].length -= 3;
		}
	}

	this->EncodeBlock(this->queuedBlocks[0], this->nUnprocessedBytesInDictionary + this->queuedBlocks[1].length + this->queuedBlocks[1].nExtraBytes);
	this->queuedBlocks[0] = this->queuedBlocks[1];
	this->nQueuedBlocks--;
}

void OC31Compressor::PutBlockInQueue(const QueuedBlock& nextBlock)
{
	if( (this->nQueuedBlocks > 0) and (!this->NewestQueuedBlock().isBackreference and !nextBlock.isBackreference) )
	{
		this->NewestQueuedBlock().length += nextBlock.length;
		return;
	}
	else if(this->nQueuedBlocks == 2)
		this->FlushTopMostBlock();

	this->queuedBlocks[this->nQueuedBlocks++] = nextBlock;
}

void OC31Compressor::WriteBackreference(uint16 distance, uint16 length, uint8 nExtraBytes, const uint8* extraBytes)
{
	DataWriter dataWriter(true, *this->baseStream);

	if((length <= 8) && (distance <= 0x7FF))
	{
		//pre-condition: length >= 3
		uint8 flagByte = ((length - 1) << 5) | (distance >> 6);
		this->WriteFlagByte(flagByte);

		uint8 packed = (distance & 0x3f) << 2;
		packed |= nExtraBytes;
		this->WriteByte(packed);
	}
	else if(distance < 0x4000)
	{
		//length = 2 could theoretically be encoded using this, but that would require 3 bytes at least, which is the same as a raw block

		if(length <= (0x1F + 2))
			this->WriteFlagByte((length - 2) | 0x20);
		else if( (length >= 0x22) and (length <= (Unsigned<uint8>::Max() + 0x22)) )
		{
			this->WriteFlagByte(0x20);
			this->WriteByte(length - 0x22);
		}
		else
		{
			this->WriteFlagByte(0x21);
			dataWriter.WriteUInt16(length);
		}

		dataWriter.WriteUInt16((distance << 2) | nExtraBytes);
	}
	else
	{
		bool isLarge = distance >= 0x8000;
		if(length <= (7+2))
		{
			uint8 flagByte = length - 2;
			if(isLarge)
				flagByte |= 8;
			this->WriteFlagByte(flagByte | 0x10);
		}
		else if( (length >= 0xA) and (length <= (Unsigned<uint8>::Max() + 0xA)) )
		{
			uint8 flagByte = 0;
			if(isLarge)
				flagByte |= 8;
			this->WriteFlagByte(flagByte | 0x10);
			this->WriteByte(length - 0xA);
		}
		else
		{
			uint8 flagByte = 1;
			if(isLarge)
				flagByte |= 8;
			this->WriteFlagByte(flagByte | 0x10);
			dataWriter.WriteUInt16(length);
		}

		distance -= 0x4000;
		if(isLarge)
			distance -= 0x4000;

		dataWriter.WriteUInt16((distance << 2) | nExtraBytes);
	}

	for(uint8 i = 0; i < nExtraBytes; i++)
		dataWriter.WriteByte(extraBytes[i]);
}

void OC31Compressor::WriteUncompressedBlock(const uint8 *data, uint16 length)
{
	if( (length >= 4) and (length <= 17) )
	{
		this->WriteFlagByte(length - 2);
		this->baseStream->WriteBytes(data, length);
	}
	else if( (length >= 0x12) and (length <= (0x12 + Unsigned<uint8>::Max())) )
	{
		this->WriteFlagByte(0);
		this->WriteByte(length - 0x12);
		this->baseStream->WriteBytes(data, length);
	}
	else
	{
		DataWriter dataWriter(true, *this->baseStream);

		this->WriteFlagByte(1);
		dataWriter.WriteUInt16(length);
		this->baseStream->WriteBytes(data, length);
	}
}
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

static RedundancyBasedCompressor::Config OC31Config()
{
	RedundancyBasedCompressor::Config c;

	c.maxDistance = 0xBFFF;
	c.maxBackrefLength = Unsigned<uint16>::Max();
	c.initLength = 0;
	c.minBackrefLength = 3; //the encoding requires a backref to have a min length of 3, some can only be encoded with 4

	return c;
}

//Constructor
OC31Compressor::OC31Compressor(UniquePointer<SeekableOutputStream> &&baseStream) : RedundancyBasedCompressor(*baseStream, OC31Config()), baseStream(Move(baseStream))
{
	this->checkValue = 0;
	this->uncompressedSize = 0;
	this->nQueuedBlocks = 0;

	StdXX::TextWriter fourccWriter(*this->baseStream, StdXX::TextCodecType::ASCII);
	fourccWriter.WriteString(u8"OC31");

	this->uncompressedSizeOffset = this->baseStream->QueryCurrentOffset();
	this->baseStream->WriteInt32LE(0); //reserve space for the uncompressed size
}

//Public methods
void OC31Compressor::Finalize()
{
	RedundancyBasedCompressor::Finalize();

	DataWriter dataWriter(false, *this->baseStream);

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

uint32 OC31Compressor::WriteBytes(const void *source, uint32 size)
{
	const uint8* src = static_cast<const uint8 *>(source);
	for(uint32 i = 0; i < size; i++)
	{
		this->checkValue = this->checkValue xor (*src++);
	}

	this->uncompressedSize += size;
	return RedundancyBasedCompressor::WriteBytes(source, size);
}

//Private methods
int32 OC31Compressor::CheckPrimaryBackreference(uint16 distance, uint16 length)
{
	if((length == 3) and (distance > 0x7FF))
		return -1;

	//try to find a longer match within that backreference. Only need to search to 5 (max length of a backreference) because beyond that a backreference always pays out
	return 5_u16;
}

bool OC31Compressor::CheckSecondaryBackreference(uint16 d1, uint16 l1, uint16 d2, uint16 l2, uint16 distanceBetweenBackreferences)
{
	uint32 backref1Count = this->ComputeRequiredBytesForBackreference(d1, l1);

	uint16 notTakenLength = l2 - (l1 - distanceBetweenBackreferences);

	//compute the "rest" of the matched longer reference, which can either be stored as another backreference or as extra bytes
	// well for total optimization it actually could matter how we choose them, since you could balance the two overlapping backreferences
	//to achieve minimal encoding lengths, but this is not implemented here

	uint32 notTakenRestEncoding = (notTakenLength >= 3) ? this->ComputeRequiredBytesForBackreference(d2, notTakenLength) : notTakenLength;
	uint32 byteSizeIfMatchNotTaken = backref1Count + notTakenRestEncoding; //size to store the original backreference with the rest of the remaining one

	uint8 extraByteCount = this->ComputeRequiredBytesForStoringRawBytes(distanceBetweenBackreferences); //bytes for skipping over until "i"
	uint32 backref2Count = this->ComputeRequiredBytesForBackreference(d2, l2); //bytes to encode the longer backreference
	uint32 byteSizeIfMatchTaken = extraByteCount + backref2Count;

	return byteSizeIfMatchNotTaken > byteSizeIfMatchTaken; //its better to shift some bytes in this case and take the longer backreference (in next call)
}

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

void OC31Compressor::EmitBackreference(uint16 distance, uint16 length)
{
	QueuedBlock nextBlock;
	nextBlock.distance = distance;
	nextBlock.length = length;
	nextBlock.isBackreference = true;
	nextBlock.nExtraBytes = 0;
	this->PutBlockInQueue(nextBlock);
}

void OC31Compressor::EmitDirectByte()
{
	QueuedBlock nextBlock;
	nextBlock.length = 1;
	nextBlock.isBackreference = false;
	this->PutBlockInQueue(nextBlock);
}

void OC31Compressor::EncodeBlock(const QueuedBlock &block, uint16 dataDistance)
{
	if(block.isBackreference)
	{
		uint8 buffer[3];
		this->ReadSlidingWindowPart(buffer, dataDistance + block.nExtraBytes, block.nExtraBytes);
		this->WriteBackreference(block.distance - 1, block.length, block.nExtraBytes, buffer);
	}
	else
	{
		uint8 buffer[StdXX::Unsigned<uint16>::Max()];
		this->ReadSlidingWindowPart(buffer, dataDistance + block.length, block.length);
		this->WriteUncompressedBlock(buffer, block.length);
	}
}

void OC31Compressor::FlushTopMostBlock()
{
	if(this->queuedBlocks[0].isBackreference and !this->queuedBlocks[1].isBackreference)
	{
		if(this->queuedBlocks[1].length <= 3)
		{
			this->queuedBlocks[0].nExtraBytes = this->queuedBlocks[1].length;
			this->EncodeBlock(this->queuedBlocks[0], 0);
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

	this->EncodeBlock(this->queuedBlocks[0], this->queuedBlocks[1].length + this->queuedBlocks[1].nExtraBytes);
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
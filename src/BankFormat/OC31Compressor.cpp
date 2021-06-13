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
uint32 OC31Compressor::ComputeEncodedSize(const QueuedBlock &block) const
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
	}

	if(!nextBlock.isBackreference)
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

		/*BufferInputStream bufferInputStream(buffer, block.length);
		TextReader textReader(bufferInputStream, TextCodecType::Latin1);
		stdOut << u8"backref dist:" << block.distance << u8" len:" << block.length << u8" nExtra:" << block.nExtraBytes << u8" extra:" << textReader.ReadStringBySize(block.nExtraBytes) << u8" size:" << this->ComputeEncodedSize(block) << endl;*/
	}
	else
	{
		uint8 buffer[StdXX::Unsigned<uint16>::Max()];
		this->dictionary.Read(buffer, dataDistance + block.length, block.length);
		this->WriteUncompressedBlock(buffer, block.length);

		/*stdOut << u8"direct len:" << block.length << u8" size:" << this->ComputeEncodedSize(block) << endl;
		BufferInputStream bufferInputStream(buffer, block.length);
		TextReader textReader(bufferInputStream, TextCodecType::Latin1);
		stdOut << textReader.ReadStringBySize(block.length) << endl;*/
	}
}

void OC31Compressor::FindOptimalBlock(const QueuedBlock& nextBlock)
{
	/*if(nextBlock.isBackreference)
	{
		this->dictionary.IndexUpTo(this->nUnprocessedBytesInDictionary - 1);

		QueuedBlock bestBlock = nextBlock;
		for(uint16 i = 1; i < Math::Min(uint16(nextBlock.length + 1), this->nUnprocessedBytesInDictionary); i++)
		{
			this->dictionary.IndexUpTo(this->nUnprocessedBytesInDictionary - i - 1);
			auto match = this->dictionary.FindLongestMatchAtSplitDistance(this->nUnprocessedBytesInDictionary - i);

			if( (match.length > (nextBlock.length + 3)) and (nextBlock.distance != (match.distance - i)) )
			{
				bestBlock.length = i;
				bestBlock.isBackreference = bestBlock.length >= 3;
				break;
			}
		}

		this->PutBlockInQueue(bestBlock);
		this->nUnprocessedBytesInDictionary -= bestBlock.length;
	}
	else
	{
		this->PutBlockInQueue(nextBlock);
		this->nUnprocessedBytesInDictionary -= nextBlock.length;
		this->dictionary.IndexUpTo(this->nUnprocessedBytesInDictionary);
	}*/
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
		uint8 flagByte = ((length - 1) << 5) | (distance >> 6);
		this->WriteFlagByte(flagByte);

		uint8 packed = (distance & 0x3f) << 2;
		packed |= nExtraBytes;
		this->WriteByte(packed);
	}
	else if(distance < 0x4000)
	{
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
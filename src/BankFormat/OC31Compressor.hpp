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
#include <StdXX.hpp>

namespace libKORG
{
	struct QueuedBlock
	{
		bool isBackreference;
		uint16 distance;
		uint16 length;
		uint8 nExtraBytes;
	};

	class OC31Compressor : public StdXX::RedundancyBasedCompressor
	{
	public:
		//Constructor
		OC31Compressor(StdXX::UniquePointer<StdXX::SeekableOutputStream>&& baseStream);

		//Methods
		void Finalize() override;
		uint32 WriteBytes(const void *source, uint32 size) override;

	private:
		//Members
		uint8 checkValue;
		uint32 uncompressedSize;
		uint64 uncompressedSizeOffset;
		uint8 nQueuedBlocks;
		QueuedBlock queuedBlocks[2];
		StdXX::UniquePointer<StdXX::SeekableOutputStream> baseStream;

		//Methods
		int32 CheckPrimaryBackreference(uint16 distance, uint16 length) override;
		bool CheckSecondaryBackreference(uint16 d1, uint16 l1, uint16 d2, uint16 l2, uint16 distanceBetweenBackreferences) override;
		uint32 ComputeRequiredBytesForBackreference(uint16 distance, uint16 length) const;
		uint8 ComputeRequiredBytesForStoringRawBytes(uint8 bytesCount) const;
		void EmitBackreference(uint16 distance, uint16 length) override;
		void EmitDirectByte() override;
		void EncodeBlock(const QueuedBlock& block, uint16 dataDistance);
		void FindOptimalBlock(const QueuedBlock& block);
		void FlushTopMostBlock();
		void PutBlockInQueue(const QueuedBlock& block);
		void WriteBackreference(uint16 distance, uint16 length, uint8 nExtraBytes, const uint8* extraBytes);
		void WriteUncompressedBlock(const uint8* data, uint16 length);

		//Inline
		inline QueuedBlock& NewestQueuedBlock()
		{
			return this->queuedBlocks[this->nQueuedBlocks - 1];
		}

		inline void WriteByte(uint8 byte)
		{
			this->baseStream->WriteBytes(&byte, 1);
		}

		inline void WriteFlagByte(uint8 flagByte)
		{
			this->WriteByte(flagByte);
		}
	};
}
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
#include <StdXX.hpp>

/*
 * The block structure and in particular block 0x100 are based on the info provided by Mateusz Faderewski (https://github.com/Polprzewodnikowy/KorgPackage)
 */

class ZLibBlocksInputStream : public StdXX::BlockInputStream<1 * MiB>
{
public:
	//Constructor
	inline ZLibBlocksInputStream(StdXX::InputStream& baseInputStream, bool verify, uint64 dataSize, uint64 fileSize) : BlockInputStream(baseInputStream)
	{
		this->verify = verify;
		this->endBlockRead = false;

		this->dataSize = dataSize;
		this->fileSize = fileSize;
		this->readDataSize = 0;
		this->readFileSize = 0;
	}

	//Methods
	bool IsAtEnd() const override
	{
		return this->endBlockRead || BlockInputStream::IsAtEnd();
	}

private:
	//Members
	bool verify;
	bool endBlockRead;
	uint64 dataSize;
	uint64 fileSize;
	uint64 readDataSize;
	uint64 readFileSize;

	//Abstract
	uint32 Read0x100Block(StdXX::InputStream& baseInputStream, void* destination)
	{
		StdXX::DataReader leReader(false, baseInputStream);
		StdXX::DataReader beReader(true, baseInputStream);

		uint32 compressedSize = leReader.ReadUInt32();
		uint32 uncompressedSize = beReader.ReadUInt32();

		ASSERT(uncompressedSize <= 1 * MiB, u8"Report this please!");

		compressedSize -= 4; //subtract the uncompressed size field
		StdXX::LimitedInputStream limitedInputStream(baseInputStream, compressedSize);
		StdXX::UniquePointer<StdXX::Decompressor> decompressor = StdXX::Decompressor::Create(StdXX::CompressionStreamFormatType::zlib, limitedInputStream, this->verify);
		StdXX::BufferOutputStream bufferOutputStream(destination, uncompressedSize);

		decompressor->FlushTo(bufferOutputStream);
		ASSERT_EQUALS(uncompressedSize, bufferOutputStream.GetCurrentOffset());

		this->readDataSize += 8 + compressedSize;
		this->readFileSize += uncompressedSize;

		return uncompressedSize;
	}

	uint32 ReadNextBlock(StdXX::InputStream& baseInputStream, void* destination)
	{
		if(this->endBlockRead)
			return 0;

		StdXX::DataReader leReader(false, baseInputStream);

		uint32 blockType = leReader.ReadUInt32();
		this->readDataSize += 4;
		switch(blockType)
		{
			case 0x100:
				return this->Read0x100Block(baseInputStream, destination);

			//end of stream block
			case 0x101:
			case 0x10100:
			case 0x1010000:
				ASSERT_EQUALS(this->fileSize, this->readFileSize);
				ASSERT(StdXX::Math::IsValueInInterval(this->dataSize - this->readDataSize, 4_u64, 7u_64), u8"???");

				while(this->readDataSize < this->dataSize)
				{
					ASSERT_EQUALS(0, leReader.ReadByte())
					this->readDataSize++;
				}
				this->endBlockRead = true;
				return 0;

			case 0x1000000:
				ASSERT_EQUALS(this->fileSize, this->readFileSize);
				ASSERT_EQUALS(this->dataSize, this->readDataSize + 7);
				NOT_IMPLEMENTED_ERROR; //TODO: ?????????????????
				return 0;

			//TODO: ?????????????????
			case 0:
			case 0x10000:
				StdXX::stdErr << u8"Unknown block: " << StdXX::String::HexNumber(blockType)
							  << u8" at: " << this->readDataSize << u8" of " << this->dataSize
							  << u8" / " << this->readFileSize << u8" of " << this->fileSize
							  << StdXX::endl;
				this->endBlockRead = true; //don't understand this block
				return 0;

			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
};
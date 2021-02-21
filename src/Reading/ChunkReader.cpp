/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "ChunkReader.hpp"
//Local
#include "../BankFormat/OC31Decompressor.hpp"
//Namespaces
using namespace StdXX;

//Public methods
void ChunkReader::ReadData(InputStream &inputStream)
{
	FileSystem::File dir(this->GetDebugDirName());
	//dir.RemoveChildrenRecursively();

	this->ReadChunks(inputStream, 0);
}

//Protected methods
bool ChunkReader::IsDataChunk(const ChunkHeader &chunkHeader)
{
	ASSERT_EQUALS(0, chunkHeader.flags & (uint8)ChunkHeaderFlags::Encrypted);
	return chunkHeader.flags & (uint8)ChunkHeaderFlags::Leaf;
}

void ChunkReader::OnEnteringChunk(const ChunkHeader& chunkHeader)
{
}

void ChunkReader::OnLeavingChunk(const ChunkHeader& chunkHeader)
{
}

//Private methods
ChunkHeader ChunkReader::ReadChunkHeader(DataReader& dataReader)
{
	ChunkHeader chunkHeader{};

	chunkHeader.id = dataReader.ReadUInt32();

	chunkHeader.type = chunkHeader.id >> 24;
	chunkHeader.version.major = (chunkHeader.id >> 16) & 0xff;
	chunkHeader.version.minor = (chunkHeader.id >> 8) & 0xff;
	chunkHeader.flags = chunkHeader.id & 0xFFu;
	chunkHeader.size = dataReader.ReadUInt32();

	uint8 flagsCopy = chunkHeader.flags;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::Unknown;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::Leaf;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::OC31Compressed;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::Encrypted;

	ASSERT_EQUALS(0, flagsCopy);

	return chunkHeader;
}

void ChunkReader::ReadChunks(InputStream &inputStream, uint8 depth)
{
	DataReader dataReader(true, inputStream);

	while(!inputStream.IsAtEnd())
	{
		ChunkHeader chunkHeader = this->ReadChunkHeader(dataReader);

		ChainedInputStream chainedInputStream(new LimitedInputStream(inputStream, chunkHeader.size));
		chainedInputStream.Add(new BufferedInputStream(chainedInputStream.GetEnd()));

		if(chunkHeader.flags & (uint8)ChunkHeaderFlags::OC31Compressed)
		{
			uint32 oc31Header = this->ReadFourCC(chainedInputStream.GetEnd());
			ASSERT( (oc31Header == FOURCC(u8"OC31")) || (oc31Header == FOURCC(u8"OC32")), String::HexNumber(oc31Header));
			chainedInputStream.Add(new libKORG::OC31Decompressor(chainedInputStream.GetEnd()));
		}
		InputStream& chunkInputStream = chainedInputStream.GetEnd();

		if(this->IsDataChunk(chunkHeader))
		{
//#define PRINT_CHUNK_STRUCTURE
#ifdef PRINT_CHUNK_STRUCTURE
			this->PrintDashes(depth + 1);
			stdOut << "Reading Data chunk " << String::HexNumber(chunkHeader.id) << u8" with size " << chunkHeader.size << endl;
#endif

			DataReader chunkDataReader(true, chunkInputStream);
			this->ReadDataChunk(chunkHeader, chunkDataReader);

			if(!chunkInputStream.IsAtEnd())
			{
				static int __iteration = 0;
				stdErr << u8"Trailing data found for chunk " << String::HexNumber(chunkHeader.id) << u8". Counter: " << __iteration << endl;
				FileOutputStream fileOutputStream(
						FileSystem::Path(this->GetDebugDirName() + String::HexNumber(chunkHeader.id) + u8"_" + String::Number(__iteration++)),
						true);
				chunkInputStream.FlushTo(fileOutputStream);
			}
		}
		else
		{
#ifdef PRINT_CHUNK_STRUCTURE
			this->PrintDashes(depth + 1);
			stdOut << "Entering chunk " << String::HexNumber(chunkHeader.id) << u8" with size " << chunkHeader.size << endl;
#endif

			this->OnEnteringChunk(chunkHeader);
			this->ReadChunks(chunkInputStream, depth + 1);
			this->OnLeavingChunk(chunkHeader);

#ifdef PRINT_CHUNK_STRUCTURE
			this->PrintDashes(depth + 1);
			stdOut << "Leaving chunk " << String::HexNumber(chunkHeader.id) << endl;
#endif
		}
	}
}
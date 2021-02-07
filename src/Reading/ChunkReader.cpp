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
//Namespaces
using namespace StdXX;

//Public methods
void ChunkReader::ReadData(InputStream &inputStream)
{
	//FileSystem::File dir(this->GetDebugDirName());
	//dir.RemoveChildrenRecursively();

	this->ReadChunks(inputStream, 0);
}

//Private methods
void ChunkReader::ReadChunks(InputStream &inputStream, uint8 depth)
{
	DataReader dataReader(true, inputStream);

	while(!inputStream.IsAtEnd())
	{
		uint32 chunkId = dataReader.ReadUInt32();
		uint32 chunkSize = dataReader.ReadUInt32();

		LimitedInputStream chunkInputStream(inputStream, chunkSize);
		if(chunkId & 8)
		{
//#define PRINT_CHUNK_STRUCTURE
#ifdef PRINT_CHUNK_STRUCTURE
			this->PrintDashes(depth + 1);
			stdOut << "Reading Data chunk " << String::HexNumber(chunkId) << u8" with size " << chunkSize << endl;
#endif

			DataReader chunkDataReader(true, chunkInputStream);
			this->ReadDataChunk(chunkId, chunkDataReader);

			if(!chunkInputStream.IsAtEnd())
			{
				static int __iteration = 0;
				//stdErr << u8"Trailing data found. Counter: " << __iteration << endl;
				FileOutputStream fileOutputStream(
						FileSystem::Path(this->GetDebugDirName() + String::HexNumber(chunkId) + "_" + String::Number(__iteration++)),
						true);
				chunkInputStream.FlushTo(fileOutputStream);
			}
		}
		else
		{
#ifdef PRINT_CHUNK_STRUCTURE
			this->PrintDashes(depth + 1);
			stdOut << "Entering chunk " << String::HexNumber(chunkId) << u8" with size " << chunkSize << endl;
#endif

			this->OnEnteringChunk(chunkId);
			this->ReadChunks(chunkInputStream, depth + 1);
			this->OnLeavingChunk(chunkId);

#ifdef PRINT_CHUNK_STRUCTURE
			this->PrintDashes(depth + 1);
			stdOut << "Leaving chunk " << String::HexNumber(chunkId) << endl;
#endif
		}
	}
}

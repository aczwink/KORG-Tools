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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

class DataDumpChunkReader : public ChunkReader
{
public:
	//Constructor
	inline DataDumpChunkReader(const Path& path)
	{
		this->pathStack.Push(path);
		this->chunkOrder.Push(0);
		this->depth = 0;
	}

protected:
	//Methods
	ChunkReader* OnEnteringChunk(const ChunkHeader &chunkHeader) override
	{
		this->PrintDashes(this->depth);
		stdOut << u8"Entering chunk " << String::HexNumber(chunkHeader.id, 8) << u8" with size " << chunkHeader.size << endl;
		this->depth++;

		this->pathStack.Push(this->FormPath(chunkHeader.id));
		this->chunkOrder.Push(0);

		File dir(this->pathStack.Last());
		dir.CreateDirectory();

		return this;
	}

	void OnLeavingChunk(const ChunkHeader &chunkHeader) override
	{
		this->depth--;
		this->PrintDashes(this->depth);
		stdOut << u8"Leaving chunk " << String::HexNumber(chunkHeader.id, 8) << endl;

		this->pathStack.Pop();
		this->chunkOrder.Pop();
	}

	void ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader) override
	{
		this->PrintDashes(depth);
		stdOut << u8"Reading Data chunk " << String::HexNumber(chunkHeader.id, 8) << u8" with size " << chunkHeader.size << endl;

		Path path = this->FormPath(chunkHeader.id);

		FileOutputStream fileOutputStream(path);
		dataReader.InputStream().FlushTo(fileOutputStream);
	}

private:
	//Members
	uint8 depth;
	DynamicArray<Path> pathStack;
	DynamicArray<uint32> chunkOrder;

	//Inline
	inline Path FormPath(uint32 chunkId)
	{
		uint32 pos = this->chunkOrder.Last()++;
		return this->pathStack.Last() / (String::Number(pos) + u8"_" + String::HexNumber(chunkId, 8));
	}

	inline void PrintDashes(uint8 depth)
	{
		for(uint8 i = 0; i < depth; i++)
			StdXX::stdOut << "-";
	}
};

void DumpChunks(const Path& path, InputStream& inputStream)
{
	File dir(path);
	dir.CreateDirectory();

	DataDumpChunkReader dataDumpChunkReader(path);
	dataDumpChunkReader.ReadData(inputStream);
}
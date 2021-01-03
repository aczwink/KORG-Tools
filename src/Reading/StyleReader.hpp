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
#include <libkorg/Style.hpp>
#include "ChunkReader.hpp"

class StyleReader : public ChunkReader
{
public:
	//Methods
	libKORG::Style* TakeResult();

protected:
	//Methods
	StdXX::String GetDebugDirName() const override;
	void ReadDataChunk(uint32 chunkId, StdXX::DataReader &dataReader) override;
	void OnEnteringChunk(uint32 chunkId) override;
	void OnLeavingChunk(uint32 chunkId) override;

private:
	//Members
	uint32 parentChunkId;

	//Methods
	void Read0x1000008Chunk(StdXX::DataReader& dataReader);
	void Read0x1000308Chunk(StdXX::DataReader& dataReader);
	void Read0x1010108Chunk(StdXX::DataReader& dataReader);
	void Read0x2000008Chunk(StdXX::DataReader& dataReader);
	void Read0x2000308Chunk(StdXX::DataReader& dataReader);
	void Read0x3000008Chunk(StdXX::DataReader& dataReader);
	void Read0x4000008Chunk(StdXX::DataReader& dataReader);
	void Read0x5010008Chunk(StdXX::DataReader& dataReader);
	void ReadKORG_MIDIEvents(uint16 dataLength, StdXX::DataReader& dataReader);
	libKORG::StyleTrackData ReadStyleTrackData(StdXX::DataReader& dataReader);
};
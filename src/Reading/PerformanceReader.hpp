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
//Local
#include <libkorg/SingleTouchSettings.hpp>
#include "ChunkReader.hpp"

class PerformanceReader : public ChunkReader
{
public:
	inline PerformanceReader()
	{
		this->perfIndex = 0;
	}

	//Methods
	libKORG::Performance* TakePerformanceResult();
	libKORG::SingleTouchSettings* TakeSTSResult();

protected:
	//Methods
	StdXX::String GetDebugDirName() const override;
	void ReadDataChunk(uint32 chunkId, StdXX::DataReader &dataReader) override;
	void OnEnteringChunk(uint32 chunkId) override;
	void OnLeavingChunk(uint32 chunkId) override;

private:
	//Members
	uint32 parentChunkId;
	uint8 perfIndex;
	libKORG::AccompanimentSettings accompanimentSettings;
	StdXX::StaticArray<libKORG::KeyboardSettings, 4> keyboardSettings;

	//Methods
	void Read0x04000108Chunk(StdXX::DataReader& dataReader);
	void Read0x07000008Chunk(StdXX::DataReader& dataReader);
	void Read0x08000008Chunk(StdXX::DataReader& dataReader);
	void Read0x09000008Chunk(StdXX::DataReader& dataReader);
	void Read0x0F000008Chunk(StdXX::DataReader& dataReader);
	void Read0x10000008Chunk(StdXX::DataReader& dataReader);
	void Read0x12000108Chunk(StdXX::DataReader& dataReader);
};
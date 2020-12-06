/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "ChunkReader.hpp"

class PerformanceReader : public ChunkReader
{
protected:
	//Methods
	StdXX::String GetDebugDirName() const override;
	void ReadDataChunk(uint32 chunkId, StdXX::DataReader &dataReader) override;

private:
	//Methods
	void Read0x07000008Chunk(StdXX::DataReader& dataReader);
	void Read0x08000008Chunk(StdXX::DataReader& dataReader);
	void Read0x09000008Chunk(StdXX::DataReader& dataReader);
};
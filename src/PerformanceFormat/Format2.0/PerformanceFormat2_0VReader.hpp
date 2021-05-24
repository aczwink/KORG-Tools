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
#pragma once
//Local
#include "UnknownAdditionalReader.hpp"
#include "../Format1.0/PerformanceFormat1_0VReader.hpp"

class PerformanceFormat2_0VReader : public PerformanceFormat1_0VReader
{
public:
	//Constructor
	inline PerformanceFormat2_0VReader() : unknownAdditionalReader(generalPerformanceData)
	{
	}

protected:
	//Members
	UnknownAdditionalReader unknownAdditionalReader;

	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Methods
	void Read0x04020008Chunk(StdXX::DataReader& dataReader);
	void Read0x20000008Chunk(StdXX::DataReader& dataReader);
	void ReadUnknownSubChunk(libKORG::Performance::UnknownSubChunk& chunk, StdXX::DataReader& dataReader);
};
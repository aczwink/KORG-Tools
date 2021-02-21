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
#include <libkorg/UnknownChunk.hpp>
#include "ChunkReader.hpp"

class PerformanceReader : public ChunkReader
{
public:
	inline PerformanceReader()
	{
		this->perfIndex = 0;
		this->version.minor = 0;
		this->version.major = 0;
	}

	//Methods
	libKORG::Performance* TakePerformanceResult();
	libKORG::SingleTouchSettings* TakeSTSResult();

protected:
	//Methods
	StdXX::String GetDebugDirName() const override;
	bool IsDataChunk(const ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	uint8 perfIndex;
	ChunkVersion version;
	StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtBeginning;
	libKORG::AccompanimentSettings accompanimentSettings;
	StdXX::StaticArray<libKORG::KeyboardSettings, 4> keyboardSettings;
	StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtEnd;
};
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
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include "AccompanimentSettingsReader.hpp"
#include "KeyboardSettingsReader.hpp"

class PerformanceFormat1_0V0_0Reader : public libKORG::ChunkReader
{
public:
	inline PerformanceFormat1_0V0_0Reader() : accompanimentSettingsReader(accompanimentSettings, max9version)
	{
		this->perfIndex = 0;
		this->max9version.minor = 0;
		this->max9version.major = 0;
	}

	//Methods
	libKORG::Performance* TakePerformanceResult();
	libKORG::SingleTouchSettings* TakeSTSResult();

protected:
	//Methods
	ChunkReader &OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void OnLeavingChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	uint8 perfIndex;
	libKORG::ChunkVersion max9version;
	StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtBeginning;
	libKORG::AccompanimentSettings accompanimentSettings;
	StdXX::StaticArray<libKORG::KeyboardSettings, 4> keyboardSettings;
	StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtEnd;

	//Subreaders
	AccompanimentSettingsReader accompanimentSettingsReader;
	StdXX::UniquePointer<KeyboardSettingsReader> keyboardSettingsReader;
};
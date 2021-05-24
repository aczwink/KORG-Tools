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
#pragma once
//Local
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include <libkorg/BankFormat/PerformanceObject.hpp>

class KeyboardSettingsV0_0Reader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline KeyboardSettingsV0_0Reader(StdXX::StaticArray<libKORG::Performance::KeyboardSettings, 4> &keyboardSettings, uint8 perfIndex)
			: keyboardSettings(keyboardSettings), perfIndex(perfIndex)
	{
	}

protected:
	//Members
	StdXX::StaticArray<libKORG::Performance::KeyboardSettings, 4>& keyboardSettings;
	uint8 perfIndex;

	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void Read0x0FChunk(StdXX::DataReader &dataReader);
	void Read0x10000008Chunk(StdXX::DataReader &dataReader);
	void Read0x11000008Chunk(StdXX::DataReader &dataReader);
	void Read0x12000108Chunk(StdXX::DataReader &dataReader);
	void Read0x13000008Chunk(StdXX::DataReader &dataReader);
	void Read0x14000008Chunk(StdXX::DataReader &dataReader);
	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Methods
	void Read0x07000008Chunk(StdXX::DataReader& dataReader);
};
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
#include <libkorg/BankFormat/BankObject.hpp>
#include <libkorg/Performance/Version2/Data.hpp>
#include "../Format1.0/AccompanimentSettingsReader.hpp"
#include "../Format1.0/KeyboardSettingsReader.hpp"
#include "AccompanimentSettingsV1_0Reader.hpp"
#include "KeyboardSettingsV1_0Reader.hpp"
#include "UnknownAdditionalReaderV2.hpp"

class PerformanceFormat2_0VReader : public libKORG::BankFormat::BankObjectReader
{
public:
	//Constructor
	inline PerformanceFormat2_0VReader() : accompanimentSettingsReader(generalData.accompanimentSettings),
										   keyboardSettingsReader(keyboardSettings),
										   unknownAdditionalReader(generalData)
	{
		this->perfIndex = 0;
	}

	//Methods
	libKORG::BankFormat::BankObject *TakeResult() override;

protected:
	//Members
	libKORG::Performance::V2::GeneralData generalData;

	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void OnLeavingChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	uint8 perfIndex;
	StdXX::StaticArray<libKORG::Performance::V2::KeyboardSettings, 4> keyboardSettings;
	AccompanimentSettingsReader<libKORG::Performance::V2::AccompanimentSettings, AccompanimentSettingsV1_0Reader, 0x0100> accompanimentSettingsReader;
	KeyboardSettingsReader<libKORG::Performance::V2::KeyboardSettings, KeyboardSettingsV1_0Reader, 0x0100> keyboardSettingsReader;
	UnknownAdditionalReaderV2 unknownAdditionalReader;

	//Methods
	void Read0x04020008Chunk(StdXX::DataReader& dataReader);
	void Read0x20000008Chunk(StdXX::DataReader& dataReader);
	void ReadUnknownSubChunk(libKORG::Performance::V2::UnknownSubChunk& chunk, StdXX::DataReader& dataReader);
};
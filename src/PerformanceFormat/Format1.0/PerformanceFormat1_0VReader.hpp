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
#include <libkorg/BankFormat/SingleTouchSettings.hpp>
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include "AccompanimentSettingsReader.hpp"
#include "KeyboardSettingsReader.hpp"
#include "KeyboardSettingsV0_0Reader.hpp"
#include "UnknownAdditionalReader.hpp"

class PerformanceFormat1_0VReader : public libKORG::BankFormat::BankObjectReader
{
public:
	//Constructor
	inline PerformanceFormat1_0VReader()
		: accompanimentSettingsReader(generalData.accompanimentSettings),
		  keyboardSettingsReader(keyboardSettings),
		  unknownAdditionalReader(generalData)
	{
		this->perfIndex = 0;
	}

	//Public methods
	libKORG::BankFormat::BankObject *TakeResult() override;

protected:
	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void OnLeavingChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	uint8 perfIndex;
	AccompanimentSettingsReader<libKORG::Performance::V1::AccompanimentSettings, AccompanimentSettingsV0_0Reader, 0x0000> accompanimentSettingsReader;
	KeyboardSettingsReader<libKORG::Performance::V1::KeyboardSettings, KeyboardSettingsV0_0Reader, 0x0000> keyboardSettingsReader;
	UnknownAdditionalReader unknownAdditionalReader;
	libKORG::Performance::V1::GeneralData generalData;
	StdXX::StaticArray<libKORG::Performance::V1::KeyboardSettings, 4> keyboardSettings;

	//Methods
	void Read0x04000108Chunk(StdXX::DataReader& dataReader);
};
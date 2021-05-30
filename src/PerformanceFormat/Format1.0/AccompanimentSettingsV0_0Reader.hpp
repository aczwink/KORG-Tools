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
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include <libkorg/BankFormat/PerformanceObject.hpp>

class AccompanimentSettingsV0_0Reader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline AccompanimentSettingsV0_0Reader(libKORG::Performance::V1::AccompanimentSettings& accompanimentSettings)
			: accompanimentSettings(accompanimentSettings)
	{
	}

protected:
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	libKORG::Performance::V1::AccompanimentSettings& accompanimentSettings;
};
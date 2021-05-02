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
#include <libkorg/BankFormat/Performance.hpp>

class AccompanimentSettingsReader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline AccompanimentSettingsReader(libKORG::AccompanimentSettings& accompanimentSettings, libKORG::ChunkVersion& max9version)
		: accompanimentSettings(accompanimentSettings), max9version(max9version)
	{
	}

protected:
	//Methods
	ChunkReader* OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;
	bool IsDataChunk(const libKORG::ChunkHeader &chunkHeader) override;

private:
	//Members
	libKORG::AccompanimentSettings& accompanimentSettings;
	libKORG::ChunkVersion& max9version;
};
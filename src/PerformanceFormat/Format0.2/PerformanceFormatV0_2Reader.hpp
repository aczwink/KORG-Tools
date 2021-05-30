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
#include <libkorg/Performance/Version0/Data.hpp>

class PerformanceFormatV0_2Reader : public libKORG::BankFormat::BankObjectReader
{
public:
	//Constructor
	inline PerformanceFormatV0_2Reader(bool stylePerformances) : stylePerformances(stylePerformances)
	{
	}

	//Methods
	libKORG::BankFormat::BankObject *TakeResult() override;

protected:
	//Members
	libKORG::Performance::V0::GeneralData generalData;
	StdXX::StaticArray<libKORG::Performance::V0::KeyboardSettings, 4> keyboardSettings;

	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void Read0x00000008Chunk(StdXX::DataReader& dataReader);
	void Read0x02Chunk(uint8 versionMinor, StdXX::DataReader &dataReader);
	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

	//Overrideable
	virtual void ReadFX(libKORG::Performance::V0::FX& fx, StdXX::DataReader& dataReader);

private:
	//Members
	bool stylePerformances;

	//Methods
	void ReadAccompanimentSettings(StdXX::DataReader &dataReader);
	void ReadEffectGroup(libKORG::Performance::V0::FXGroup& fxGroup, StdXX::DataReader &dataReader);
	void ReadKeyboardSettings(libKORG::Performance::V0::KeyboardSettings& unknownPart2, StdXX::DataReader& dataReader);
	void ReadPadSettings(libKORG::Performance::V0::PadSettings& padSettings, StdXX::DataReader& dataReader);
	void ReadTrackParams(libKORG::Performance::V0::TrackSettings& unknownPart8, StdXX::DataReader &dataReader);
};
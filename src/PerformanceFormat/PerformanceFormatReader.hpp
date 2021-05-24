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
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include <libkorg/BankFormat/PerformanceObject.hpp>
#include <libkorg/BankFormat/SingleTouchSettings.hpp>
#include <libkorg/Performance/GeneralData.hpp>
#include "KeyboardSettingsFormat/KeyboardSettingsReader.hpp"
#include "AccompanimentSettingsFormat/AccompanimentSettingsReader.hpp"

class PerformanceFormatReader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline PerformanceFormatReader() : accompanimentSettingsReader(generalPerformanceData.accompanimentSettings, max9version),
									   keyboardSettingsReader(keyboardSettings)
	{
		this->perfIndex = 0;
		this->max9version.minor = 0;
		this->max9version.major = 0;
	}

	//Destructor
	virtual ~PerformanceFormatReader() = default;

	//Methods
	libKORG::PerformanceObject* TakePerformanceResult();
	libKORG::SingleTouchSettings* TakeSTSResult();

protected:
	//Members
	uint8 perfIndex;
	libKORG::ChunkVersion max9version;
	libKORG::Performance::GeneralData generalPerformanceData;
	StdXX::StaticArray<libKORG::Performance::KeyboardSettings, 4> keyboardSettings;

	//Subreaders
	AccompanimentSettingsReader accompanimentSettingsReader;
	KeyboardSettingsReader keyboardSettingsReader;
};
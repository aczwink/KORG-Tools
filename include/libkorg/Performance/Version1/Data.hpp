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
#include "AccompanimentSettings.hpp"
#include "KeyboardSettings.hpp"

namespace libKORG::Performance::V1
{
	struct _0x04000108_chunk
	{
		uint8 memoryBankNumber = 0;
		uint8 unknown3 = 0;
		uint8 unknown4 = 0;
		uint8 metronomeTempo = 120;
		uint8 unknown41[8] = {};
		uint8 unknown5[8] = {8, 8, 8, 8, 8, 8, 8, 8};
		uint8 unknown6[8] = {1, 1, 1, 1, 1, 1, 1, 1};
		uint8 unknown7[3] = {};
		uint8 unknown8[14*3] = {};

		//Operators
		bool operator==(const _0x04000108_chunk&) const = default;
	};

	struct _0x1B000008_chunk
	{
		uint8 unknown1 = 1;
		uint8 unknown2 = 0;
		uint8 unknown3 = 0;

		//Operators
		bool operator==(const _0x1B000008_chunk&) const = default;
	};

	struct _0x1A000008_chunk
	{
		uint8 unknown1 = 1;
		uint8 unknown2 = 60;

		//Operators
		bool operator==(const _0x1A000008_chunk&) const = default;
	};

	struct GeneralData
	{
		_0x04000108_chunk _0x04000108_data;
		AccompanimentSettings accompanimentSettings;
		_0x1B000008_chunk _0x1B000008_data;
		_0x1A000008_chunk _0x1A000008_data;

		//Constructors
		GeneralData() = default;
		GeneralData(const GeneralData&) = default;
		GeneralData(GeneralData&&) = default;

		//Operators
		bool operator==(const GeneralData&) const = default;
	};

	struct PerformanceData : public GeneralData
	{
		KeyboardSettings keyboardSettings;

		//Constructors
		PerformanceData() = default;

		inline PerformanceData(GeneralData&& generalData, KeyboardSettings&& keyboardSettings)
			: GeneralData(StdXX::Move(generalData)),
			keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}

		//Operators
		bool operator==(const PerformanceData&) const = default;
	};

	struct STSData : public GeneralData
	{
		StdXX::StaticArray<KeyboardSettings, 4> keyboardSettings;

		//Constructors
		STSData() = default;

		inline STSData(GeneralData&& generalData, StdXX::StaticArray<KeyboardSettings, 4>&& keyboardSettings)
		: GeneralData(StdXX::Move(generalData)),
		keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}

		STSData(const STSData&) = default;
	};
}
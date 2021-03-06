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
#include <libkorg/Performance/Version1/Data.hpp>
#include "AccompanimentSettings.hpp"
#include "KeyboardSettings.hpp"

namespace libKORG::Performance::V2
{
	struct UnknownSubChunk
	{
		uint8 unknown1;
		uint8 unknown2;
		uint8 unknown3;
		uint8 unknown4;
		StdXX::String unknown5;
		StdXX::String unknown6;
		StdXX::DynamicArray<uint8> unknown7;
	};

	struct _0x04020008_chunk
	{
		int32 unknown1;
		UnknownSubChunk subChunk;
		uint8 unknown81;
		uint8 metronomeTempo;
		uint8 unknown83[8];
		uint8 unknown9[8];
		uint8 unknown10[8];
		uint8 unknown11[3];
		uint8 unknown12[14*3];
	};

	struct _0x20000008_chunk
	{
		int32 unknown1;
		UnknownSubChunk subChunk;
		uint8 unknown2;
		uint16 unknown3;
		uint8 unknown4[8];
		uint8 unknown5[8];
		uint8 unknown6[8];
		uint8 unknown11[3];
		uint8 unknown12[14*3];
	};

	struct _0x21000008_chunk
	{
		uint8 unknown1;
		uint8 unknown2;
		uint8 unknown3;
	};

	struct GeneralData
	{
		_0x04020008_chunk _0x04020008_data;
		_0x20000008_chunk _0x20000008_data;
		AccompanimentSettings accompanimentSettings;
		V1::_0x1B000008_chunk _0x1B000008_data;
		V1::_0x1A000008_chunk _0x1A000008_data;
		_0x21000008_chunk _0x21000008_data;
	};

	struct PerformanceData : public GeneralData
	{
		KeyboardSettings keyboardSettings;

		//Constructor
		inline PerformanceData(GeneralData&& generalData, KeyboardSettings&& keyboardSettings)
				: GeneralData(StdXX::Move(generalData)),
				  keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}
	};

	struct STSData : public GeneralData
	{
		StdXX::StaticArray<KeyboardSettings, 4> keyboardSettings;

		//Constructors
		inline STSData(GeneralData&& generalData, StdXX::StaticArray<KeyboardSettings, 4>&& keyboardSettings)
				: GeneralData(StdXX::Move(generalData)),
				  keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}
	};
}
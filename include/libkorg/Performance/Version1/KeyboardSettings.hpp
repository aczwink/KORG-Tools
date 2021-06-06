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
#include <libkorg/Pitch.hpp>
#include <libkorg/Performance/Version0/Scale.hpp>
#include "FX.hpp"
#include "GeneralTracksSettings.hpp"
#include "TrackSettings.hpp"

namespace libKORG::Performance::V1
{
	struct _0x10000008_chunk
	{
		uint8 unknown1 = 2;
		uint8 unknown2 = 1;
		uint8 unknown3 = 0;
		uint8 unknown4 = 60;
		int32 unknown5 = 0;
		uint8 unknown6 = 0;

		//Operators
		bool operator==(const _0x10000008_chunk&) const = default;
	};

	struct _0x11000008_chunk
	{
		uint8 unknown1 = 0;
		uint8 unknown2 = 0;

		//Operators
		bool operator==(const _0x11000008_chunk&) const = default;
	};

	struct _0x12000108_chunk
	{
		uint8 unknown1[3] = {0, 222, 0};
		int8 unknown2[2] = {-2, 4};
		uint8 unknown3[7] = {2, 0, 2, 1, 0, 2, 0};

		//Operators
		bool operator==(const _0x12000108_chunk&) const = default;
	};

	struct _0x14000008_chunk
	{
		uint8 unknown[5] = {};

		//Operators
		bool operator==(const _0x14000008_chunk&) const = default;
	};

	struct _0x15000108_chunk
	{
		int32 unknown = 0;

		//Operators
		bool operator==(const _0x15000108_chunk&) const = default;
	};

	struct _0x18000008_chunk
	{
		uint16 unknown1[9] = {8, 8, 8, 5, 0, 0, 0, 0, 0};
		uint8 unknown2[12] = {1, 84, 64, 0, 1, 35, 0, 1, 1, 54, 1, 25};

		//Operators
		bool operator==(const _0x18000008_chunk&) const = default;
	};

	struct KeyboardSettings : public GeneralTracksSettings
	{
		StdXX::String name;
		uint32 unknown1 = 0;

		_0x10000008_chunk _0x10000008_data;
		_0x11000008_chunk _0x11000008_data;
		_0x12000108_chunk _0x12000108_data;
		V0::ScaleSettings scaleSettings;
		_0x14000008_chunk _0x14000008_data;
		StdXX::StaticArray<FXData, 4> fx;
		StdXX::StaticArray<_0x15000108_chunk, 4> _0x15000108_data;
		StdXX::StaticArray<TrackSettings, 4> trackSettings;
		_0x18000008_chunk _0x18000008_data;
	};
}
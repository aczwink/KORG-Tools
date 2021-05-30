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
#include <libkorg/Performance/Version1/KeyboardSettings.hpp>
#include "FX.hpp"
#include "TrackSettings.hpp"

namespace libKORG::Performance::V2
{
	struct _0x22000008_chunk
	{
		uint8 unknown1;
		uint8 unknown2;
	};

	struct KeyboardSettings : public V1::GeneralTracksSettings
	{
		StdXX::String name;
		uint32 unknown1;

		V1::_0x10000008_chunk _0x10000008_data;
		V1::_0x11000008_chunk _0x11000008_data;
		V1::_0x12000108_chunk _0x12000108_data;
		V0::ScaleSettings scaleSettings;
		StdXX::StaticArray<MasterFXSettings, 2> masterFxSettings;
		V1::_0x14000008_chunk _0x14000008_data;
		V1::_0x15000108_chunk _0x15000108_data;
		StdXX::StaticArray<TrackSettings, 4> trackSettings;
		_0x22000008_chunk _0x22000008_data;
		V1::_0x18000008_chunk _0x18000008_data;
	};
}
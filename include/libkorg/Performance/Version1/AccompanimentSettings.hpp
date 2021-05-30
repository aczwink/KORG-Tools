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
#include "GeneralTracksSettings.hpp"
#include "TrackSettings.hpp"
#include "FX.hpp"

namespace libKORG::Performance::V1
{
	struct _0x06000008_chunk
	{
		int8 unknown1;
		int8 unknown2;

		//Operators
		bool operator==(const _0x06000008_chunk&) const = default;
	};

	struct AccompanimentSettings : public GeneralTracksSettings
	{
		_0x06000008_chunk _0x06000008_data;
		StdXX::StaticArray<FXData, 6> fx;
		StdXX::StaticArray<TrackSettings, 8> trackSettings;

		//Operators
		bool operator==(const AccompanimentSettings&) const = default;
	};
}
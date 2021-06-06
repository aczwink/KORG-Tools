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
#include <StdXX.hpp>

namespace libKORG::Performance::V1
{
	struct PadSettings
	{
		uint32 unknown2 = 0xFFFF00FF;
		uint8 unknown3 = 100;
		int8 unknown4 = 64;
		uint8 unknown5[4] = {127, 127, 0, 0};
		int8 unknown6[4] = {};
		uint8 unknown7 = 90;

		//Operators
		bool operator==(const PadSettings&) const = default;
		bool operator!=(const PadSettings&) const = default;
	};

	struct GeneralTracksSettings
	{
		StdXX::StaticArray<PadSettings, 4> padSettings;

		//Operators
		bool operator==(const GeneralTracksSettings&) const = default;
	};
}
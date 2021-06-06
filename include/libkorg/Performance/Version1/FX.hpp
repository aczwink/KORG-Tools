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
	struct FXData
	{
		int8 unknown2 = -1;
		int8 unknown3 = -1;
		uint8 unknown4 = 0;
		uint8 unknown5 = StdXX::Unsigned<uint8>::Max();
		int8 unknown6 = 0;
		uint8 unknown7 = StdXX::Unsigned<uint8>::Max();
		uint8 unknown8 = StdXX::Unsigned<uint8>::Max();
		uint8 unknown9 = 0;
		int16 unknown10 = 0;
		uint8 unknown11[64] = {};

		//Operators
		bool operator==(const FXData&) const = default;
		bool operator!=(const FXData&) const = default;
	};
}
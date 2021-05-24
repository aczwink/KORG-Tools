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

namespace libKORG::Performance
{
	struct PadSettings
	{
		uint32 unknown2;
		uint8 unknown3;
		int8 unknown4;
		uint8 unknown5[4];
		int8 unknown6[4];
		uint8 unknown7;
	};

	struct GeneralPerformanceSettings
	{
		StdXX::StaticArray<PadSettings, 4> padSettings;

		//Constructors
		GeneralPerformanceSettings() = default;
		GeneralPerformanceSettings(const GeneralPerformanceSettings& generalPerformanceSettings) = default;
	};
}
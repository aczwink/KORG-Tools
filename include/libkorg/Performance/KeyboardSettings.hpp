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

namespace libKORG::Performance
{
	struct KeyboardSettings : public GeneralPerformanceSettings
	{
		StdXX::String name;
		uint32 unknown1;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			int32 unknown5;
			uint8 unknown6;
		} _0x10000008_chunk;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
		} _0x11000008_chunk;

		struct
		{
			uint8 unknown1[3];
			int8 unknown2[2];
			uint8 unknown3[7];
		} _0x12000108_chunk;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			int8 unknown3[12];
		} _0x13000008_chunk;

		struct
		{
			uint8 unknown[5];
		} _0x14000008_chunk;

		struct
		{
			int32 unknown;
		} _0x15000108_chunk;

		struct
		{
			uint16 unknown1[9];
			uint8 unknown2[12];
		} _0x18000008_chunk;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
		} _0x22000008_chunk;

		StdXX::StaticArray<MasterFXSettings, 2> masterFxSettings;
		StdXX::StaticArray<TrackProperties, 4> trackProperties;
	};
}
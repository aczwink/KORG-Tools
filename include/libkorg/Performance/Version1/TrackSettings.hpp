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
#include <libkorg/ProgramChangeSequence.hpp>

namespace libKORG::Performance::V1
{
	struct TrackSettings
	{
		uint32 unknown1;
		uint8 unknown2[4];
		ProgramChangeSequence soundProgramChangeSeq;
		uint8 volume;
		int8 pan;
		int8 detune;
		int8 octaveTranspose;
		uint8 unknown3[11];
		uint8 fxMaster1;
		uint8 fxMaster2;
		uint8 dry;
		uint32 unknown12;
		uint8 pbSensitivity;
		uint8 unknown182;
		int8 unknown181;
		int8 lowGainTimes2;
		int8 midGainTimes2;
		int8 highGainTimes2;
		uint8 unknown6[4];
		uint32 unknown4[4];
		uint8 unknown5[8][22];

		//Operators
		bool operator==(const TrackSettings&) const = default;
		bool operator!=(const TrackSettings&) const = default;
	};
}
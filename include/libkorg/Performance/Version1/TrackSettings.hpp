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
	struct UnknownBlock
	{
		uint8 unknown[22] = {127, 0, 64, 64, 64, 64, 64, 64, 64, 64, 0, 64, 64, 0, 0, 0, 90, 64, 64, 64, 64, 64};

		//Operators
		bool operator==(const UnknownBlock&) const = default;
	};

	struct TrackSettings
	{
		uint32 unknown1 = 768;
		uint8 unknown2[4] = {0, 127, 0, 127};
		ProgramChangeSequence soundProgramChangeSeq;
		uint8 volume = 127;
		int8 pan = 0;
		int8 detune = 0;
		int8 octaveTranspose = 0;
		uint8 unknown3[11] = {64, 64, 64, 64, 64, 64, 64, 64, 1, 0, 0};
		uint8 fxMaster1 = 127;
		uint8 fxMaster2 = 127;
		uint8 dry = 0;
		uint32 unknown12 = 16647;
		uint8 pbSensitivity = 2;
		uint8 unknown182 = 0;
		int8 unknown181 = 0;
		int8 lowGainTimes2 = 0;
		int8 midGainTimes2 = 0;
		int8 highGainTimes2 = 0;
		uint8 unknown6[4] = {90, 0, 0, 0};
		uint32 unknown4[4] = { StdXX::Unsigned<uint32>::Max(), StdXX::Unsigned<uint32>::Max(), StdXX::Unsigned<uint32>::Max(), StdXX::Unsigned<uint32>::Max() };
		UnknownBlock unknown5[8];

		//Operators
		bool operator==(const TrackSettings&) const = default;
		bool operator!=(const TrackSettings&) const = default;
	};
}
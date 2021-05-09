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

namespace libKORG::MultiSamples
{
	struct DrumSampleEntry
	{
		uint8 unknown1[12];
		StdXX::String name;
		uint8 unknown2[20];
	};

	struct KeyboardZone
	{
		uint8 unknown1;
		uint8 sampleNumber;
		Pitch to;
		Pitch originalNote;
		uint8 unknown3[2];
		int8 pitch;
		int8 level;
		uint8 unknown4;
		uint8 unknown5;
	};

	struct MultiSampleEntry
	{
		uint8 unknown1;
		StdXX::String name;
		uint8 unknown2[2];
		uint8 keyZoneIndex[128];
		uint8 nKeyZones;
		uint8 unknown3;
		uint64 id;
	};

	struct SampleEntry
	{
		uint8 unknown1[5];
		uint8 unknown6; //bits per sample?
		uint8 unknown2[62];
		StdXX::String name;
		uint64 id;
		uint8 unknown4[6];
		libKORG::Pitch originalNote;
	};

	struct MultiSamplesData
	{
		StdXX::DynamicArray<SampleEntry> sampleEntries;
		StdXX::DynamicArray<DrumSampleEntry> drumSampleEntries;
		StdXX::DynamicArray<KeyboardZone> keyboardZones;
		StdXX::DynamicArray<MultiSampleEntry> multiSampleEntries;
	};
}
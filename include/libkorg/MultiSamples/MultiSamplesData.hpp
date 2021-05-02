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

	struct MultiSampleEntry
	{
		uint8 unknown1;
		StdXX::String name;
		uint8 unknown2[140];
	};

	struct SampleEntry
	{
		uint8 unknown1[5];
		uint8 unknown6; //bits per sample?
		uint8 unknown2[62];
		StdXX::String name;
		uint8 unknown3[8]; //same as the first 8 bytes in the PCM file
		uint8 unknown4[6];
		libKORG::Pitch originalNote;
	};

	struct MultiSamplesData
	{
		StdXX::DynamicArray<SampleEntry> sampleEntries;
		StdXX::DynamicArray<DrumSampleEntry> drumSampleEntries;
		StdXX::DynamicArray<MultiSampleEntry> multiSampleEntries;
	};
}
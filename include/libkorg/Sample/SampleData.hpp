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

namespace libKORG::Sample
{
	enum class SampleFormat
	{
		Linear_PCM_S16BE = 16,
		Compressed = 128
	};

	struct SampleData
	{
		uint64 id;
		SampleFormat sampleFormat;
		uint32 sampleRate;
		uint32 nSamples;
		uint32 unknown1;
		uint32 unknown2[7];
		uint32 loopStart;
		uint32 unknown3;
		uint32 unknown4;
		uint32 unknown5;
		StdXX::DynamicByteBuffer sampleBuffer;

		//Operators
		bool operator==(const SampleData&) const = default;
	};
}
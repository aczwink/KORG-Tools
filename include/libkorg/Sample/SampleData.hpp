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

namespace libKORG::Sample
{
	enum class SampleFormat
	{
		S16BE = 16,
		S8 = 128
	};

	struct SampleData
	{
		uint8 unknown1[2];
		uint8 id[2];
		uint8 unknown11[4];
		SampleFormat sampleFormat;
		uint32 sampleRate;
		uint8 unknown2[48];
		StdXX::DynamicByteBuffer sampleBuffer;
	};
}
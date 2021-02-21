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
using namespace StdXX;

namespace BankFormat
{
	enum class ChunkType
	{
		Container = 1,
		KorgFile = 2,
		ObjectTOC = 5,
		StyleObject = 6,
		PerformancesData = 9,
		CrossReferenceTable = 0xFE,
	};
	enum class ChunkId : uint32
	{
		OldSoundDataMaybe = 0x70003,
		SongBookListData = 0x080000,
		SongBookListData1 = 0x080001,
		PerformanceData02 = 0x090002,
		PerformanceData03 = 0x090003,
		PerformanceData10 = 0x090100,
		PerformanceData20 = 0x090200,
		PerformanceData21 = 0x090201,
		PadData = 0x0C0000,
		SoundData00 = 0x100000,
		SoundData0 = 0x100100,
		SoundData1 = 0x100200,
		SoundData2 = 0x100300,
		MultiSampleData = 0x110300,
		PCMData = 0x120000,
	};

	enum class ObjectType
	{
		Performance = 1,
		Style = 2,
		Sound = 4,
		MultiSample = 5,
		PCM = 6,
		StylePerformances = 7,
		PAD = 9,
		SongBookEntry = 11,
		SongBook = 12,
	};

	struct HeaderEntry
	{
		String name;
		ObjectType type;
		uint8 pos;
	};

	const uint32 OBJECTTOC_LINESIZE = 24;
	const uint32 HEADERENTRY_NAME_SIZE = 18;
}
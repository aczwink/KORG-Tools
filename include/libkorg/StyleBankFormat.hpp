/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <StdXX.hpp>
using namespace StdXX;

namespace KorgFormat
{
	enum class ChunkId : uint32
	{
		Container = 0x01000114,
		KorgFile = 0x02000018,
		ObjectTOC = 0x05000018,
		ObjectTOC_Extended = 0x05010018,
		StyleData = 0x06000030,
		PerformanceData = 0x09010030,
		PerformanceData_Extended = 0x09020030,
		PerformanceData_Extended2 = 0x09020130
	};

	struct ChunkHeader
	{
		uint32 id;
		uint32 size;
	};

	enum class ObjectType
	{
		Performance = 1,
		Style = 2,
		StylePerformances = 7
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
/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "PerformanceFormatWriter.hpp"
#include "Format0.3/PerformanceFormatV0_3Writer.hpp"

PerformanceFormatWriter* CreatePerformanceWriter(StdXX::SeekableOutputStream& outputStream, const ChunkVersion& formatVersion)
{
	switch(formatVersion.AsUInt16())
	{
		case 0x0003:
			return new PerformanceFormatV0_3Writer(outputStream);
		case 0x0100:
			return new PerformanceWriterV1_0Writer(outputStream);
	}

	return nullptr;
}
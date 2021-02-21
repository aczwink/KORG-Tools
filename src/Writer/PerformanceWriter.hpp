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
#include <StdXX.hpp>
//Local
#include <libkorg/SingleTouchSettings.hpp>
#include "ChunkWriter.hpp"

class PerformanceWriter : public ChunkWriter
{
public:
	//Constructor
	inline PerformanceWriter(StdXX::SeekableOutputStream& outputStream) : ChunkWriter(outputStream)
	{
	}

	void Write(const libKORG::SingleTouchSettings &settings);

private:
	//Methods
	void Write0x2000000Chunk(const libKORG::AccompanimentSettings& settings, const libKORG::ChunkVersion& performanceVersion);
	void Write0x3000000Chunk(const libKORG::KeyboardSettings& keyboardSettings, uint8 number, const libKORG::ChunkVersion& performanceVersion);
	void WriteTrackProperties(uint32 trackNumber, const libKORG::TrackProperties& trackProperties, const libKORG::ChunkVersion& performanceVersion);
};
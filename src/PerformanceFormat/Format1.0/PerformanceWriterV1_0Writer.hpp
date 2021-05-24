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
#include <libkorg/BankFormat/SingleTouchSettings.hpp>
#include "../../Writer/ChunkWriter.hpp"
#include "../PerformanceFormatWriter.hpp"

class PerformanceWriterV1_0Writer : public ChunkWriter, public PerformanceFormatWriter
{
public:
	//Constructor
	inline PerformanceWriterV1_0Writer(StdXX::SeekableOutputStream& outputStream) : ChunkWriter(outputStream)
	{
	}

	void Write(const libKORG::SingleTouchSettings &settings) override;

private:
	//Methods
	void Write0x2000000Chunk(const libKORG::Performance::AccompanimentSettings& settings, const libKORG::ChunkVersion& performanceVersion);
	void Write0x3000000Chunk(const libKORG::Performance::KeyboardSettings& keyboardSettings, uint8 number, const libKORG::ChunkVersion& performanceVersion);
	void WriteTrackProperties(uint32 trackNumber, const libKORG::Performance::TrackProperties& trackProperties, const libKORG::ChunkVersion& performanceVersion);
};
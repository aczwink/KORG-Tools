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
#include "libkorg/ChunkFormat/ChunkWriter.hpp"
#include "../PerformanceFormatWriter.hpp"

class PerformanceWriterV1_0Writer : public libKORG::ChunkWriter, public PerformanceFormatWriter
{
public:
	//Constructor
	inline PerformanceWriterV1_0Writer(StdXX::SeekableOutputStream& outputStream) : ChunkWriter(outputStream)
	{
	}

	//Methods
	void Write(const libKORG::PerformanceObject &performanceObject) override;
	void Write(const libKORG::SingleTouchSettings &settings) override;

private:
	//Methods
	void Write0x02000000Chunk(const libKORG::Performance::V1::AccompanimentSettings& settings);
	void Write0x03000000Chunk(const libKORG::Performance::V1::KeyboardSettings& keyboardSettings, uint8 number);
	void Write0x04000108Chunk(const libKORG::Performance::V1::_0x04000108_chunk& _0x04000108_data);
	void Write0x06000008Chunk(const libKORG::Performance::V1::_0x06000008_chunk& chunk);
	void Write0x07000008Chunk(int32 index, const libKORG::Performance::V1::FXData& fxData);
	void Write0x08000008Chunk(int32 index, const libKORG::Performance::V1::PadSettings& padSettings);
	void Write0x09000008Chunk(int32 index, const libKORG::Performance::V1::TrackSettings& trackSettings);
	void Write0x10000008Chunk(const libKORG::Performance::V1::_0x10000008_chunk& chunk);
	void Write0x11000008Chunk(const libKORG::Performance::V1::_0x11000008_chunk& chunk);
	void Write0x12000108Chunk(const libKORG::Performance::V1::_0x12000108_chunk& chunk);
	void Write0x13000008Chunk(const libKORG::Performance::V0::ScaleSettings& scaleSettings);
	void Write0x14000008Chunk(const libKORG::Performance::V1::_0x14000008_chunk& chunk);
	void Write0x15000108Chunk(int32 index, const libKORG::Performance::V1::_0x15000108_chunk& chunk);
	void Write0x18000008Chunk(const libKORG::Performance::V1::_0x18000008_chunk& chunk);
	void Write0x1A000008Chunk(const libKORG::Performance::V1::_0x1A000008_chunk& chunk);
	void Write0x1B000008Chunk(const libKORG::Performance::V1::_0x1B000008_chunk& chunk);
	void WriteGeneralEnd(const libKORG::Performance::V1::GeneralData& generalData);
	void WriteGeneralStart(const libKORG::Performance::V1::GeneralData& generalData);
};
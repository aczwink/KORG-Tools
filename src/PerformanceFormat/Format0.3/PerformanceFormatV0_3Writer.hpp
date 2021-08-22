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
#include <libkorg/ChunkFormat/ChunkWriter.hpp>
#include "../PerformanceFormatWriter.hpp"

class PerformanceFormatV0_3Writer : public libKORG::ChunkWriter, public PerformanceFormatWriter
{
public:
	//Constructor
	inline PerformanceFormatV0_3Writer(StdXX::SeekableOutputStream& outputStream) : ChunkWriter(outputStream)
	{
	}

	void Write(const libKORG::PerformanceObject &performanceObject) override;
	void Write(const libKORG::SingleTouchSettings &singleTouchSettings) override;

private:
	//Methods
	void Write0x00010008Chunk(const libKORG::Performance::V0::GeneralData& generalData, const StdXX::DynamicArray<libKORG::Performance::V0::KeyboardSettings>& keyboardSettings);
	void Write0x02000108Chunk(const libKORG::Performance::V0::GeneralData& performanceData);
	void WriteAccompanimentSettings(const libKORG::Performance::V0::GeneralData& generalData);
	void WriteEffectGroup(const libKORG::Performance::V0::FXGroup& fxGroup);
	void WriteKeyboardSettings(const libKORG::Performance::V0::KeyboardSettings& keyboardSettings);
	void WritePadSettings(const libKORG::Performance::V0::PadSettings& padSettings);
	void WriteScaleSettings(const libKORG::Performance::V0::ScaleSettings& scaleSettings);
	void WriteTrackSettings(const libKORG::Performance::V0::TrackSettings& trackSettings);
};
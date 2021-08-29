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
#include <StdXX.hpp>
#include <libkorg/Sound/SoundData.hpp>
#include "../SoundFormatWriter.hpp"

class SoundFormat3_0Writer : public SoundFormatWriter
{
public:
	//Constructor
	inline SoundFormat3_0Writer(StdXX::DataWriter& dataWriter) : dataWriter(dataWriter)
	{
	}

	//Methods
	void Write(const libKORG::Sound::SoundData& soundData);

private:
	//Members
	StdXX::DataWriter& dataWriter;

	//Methods
	void WriteDrumKitSoundData(const libKORG::Sound::DrumKitSoundData& drumKitSoundData);
	void WriteEqualizerData(const libKORG::Sound::EqualizerData& equalizerData);
	void WriteKeyTableEntry(const libKORG::Sound::KeyTableEntry& keyTableEntry);
	void WriteLayerEntry(const libKORG::Sound::LayerEntry& layerEntry);
	void WriteOscillatorData(const libKORG::Sound::OscillatorData& oscillatorData);
	void WriteOscillatorMultiSamplesData(const libKORG::Sound::OSCMultiSampleSettings& oscMultiSampleSettings);
};
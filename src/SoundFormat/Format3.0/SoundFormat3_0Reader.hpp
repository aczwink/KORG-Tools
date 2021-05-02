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
#include <libkorg/Sound/SoundData.hpp>

class SoundFormat3_0Reader
{
public:
	//Members
	libKORG::Sound::SoundData data;

	//Methods
	void Read(StdXX::DataReader& dataReader);

private:
	//Methods
	void ReadDrumKitSoundData(libKORG::Sound::DrumKitSoundData& drumKitSoundData, StdXX::DataReader& dataReader);
	void ReadKeyTableEntry(libKORG::Sound::KeyTableEntry& keyTableEntry, StdXX::DataReader& dataReader);
	void ReadLayerEntry(libKORG::Sound::LayerEntry& layerEntry, StdXX::DataReader& dataReader);
	void ReadOscillatorData(libKORG::Sound::OSCMultiSampleSettings& oscMultiSampleSettings, StdXX::DataReader& dataReader);

	//Inline
	inline bool ReadBool(StdXX::DataReader& dataReader)
	{
		uint8 value = dataReader.ReadByte();
		ASSERT(value <= 1, StdXX::String::Number(value));
		return value == 1;
	}
};
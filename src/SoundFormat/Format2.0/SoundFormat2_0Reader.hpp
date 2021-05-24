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
#pragma once
//Local
#include "../Format1.0/SoundFormat1_0Reader.hpp"

class SoundFormat2_0Reader : public SoundFormat1_0Reader
{
public:
	//Methods
	//void Read(StdXX::DataReader& dataReader);

protected:
	//Methods
	void ReadEffects(StdXX::DataReader &dataReader) override;
	void ReadFX(libKORG::Sound::EffectData& effectData, StdXX::DataReader& dataReader) override;
	void ReadOscillatorEqualizer(libKORG::Sound::OscillatorData &oscillatorData, StdXX::DataReader &dataReader) override;
	void ReadOscillatorMultiSamplesData(libKORG::Sound::OSCMultiSampleSettings &oscMultiSampleSettings, StdXX::DataReader &dataReader) override;
	void ReadUnknowns1(StdXX::DataReader &dataReader) override;
	void ReadUnknowns2(libKORG::Sound::OscillatorData &oscillatorData, StdXX::DataReader &dataReader) override;
	void ReadUnknowns4(libKORG::Sound::KeyTableEntry &keyTableEntry, StdXX::DataReader &dataReader) override;
	void ReadLayerEntryDetails(libKORG::Sound::LayerEntry &layerEntry, StdXX::DataReader &dataReader) override;
	void SkipUnknownKeyTableEntryByte(StdXX::DataReader &dataReader) override;

private:
	//Methods
	void ReadEqualizerData(libKORG::Sound::EqualizerData& equalizerData, StdXX::DataReader& dataReader);
};
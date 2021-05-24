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
#include <StdXX.hpp>
//Local
#include <libkorg/Sound/SoundData.hpp>

class LegacySoundFormat0_0Reader
{
public:
	//Members
	libKORG::Sound::SoundData data;

	//Methods
	void Read(StdXX::DataReader& dataReader);

protected:
	//Overrideable
	virtual void ReadDrumKitSoundData(libKORG::Sound::DrumKitSoundData& drumKitSoundData, StdXX::DataReader& dataReader);
	virtual void ReadEffects(StdXX::DataReader& dataReader);
	virtual void ReadFX(libKORG::Sound::EffectData& effectData, StdXX::DataReader& dataReader);
	virtual void ReadKeyRange(libKORG::Sound::KeyRange& keyRange, StdXX::DataReader& dataReader);
	virtual uint8 ReadLowPriority(StdXX::DataReader& dataReader);
	virtual void ReadMaxTimeAndRange(StdXX::DataReader& dataReader);
	virtual uint64 ReadMultiSampleId(StdXX::DataReader& dataReader);
	virtual uint16 ReadMultiSampleMap(StdXX::DataReader& dataReader);
	virtual void ReadName(StdXX::DataReader& dataReader);
	virtual void ReadOscillatorEqualizer(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);
	virtual void ReadOscillatorMultiSamplesData(libKORG::Sound::OSCMultiSampleSettings& oscMultiSampleSettings, StdXX::DataReader& dataReader);
	virtual void ReadOSCTrigger(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);
	virtual void ReadScaledVelocity(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);
	virtual void ReadUnknowns1(StdXX::DataReader& dataReader);
	virtual void ReadUnknowns2(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);
	virtual void ReadUnknowns10(StdXX::DataReader& dataReader);
	virtual void ReadUnknowns11(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);
	virtual void ReadUnknowns21(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);

private:
	//Methods
	void ReadOscillatorData(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader& dataReader);
	void ReadUnknownData(StdXX::DataReader& dataReader);
	void ReadUnknowns20(StdXX::DataReader& dataReader);
};
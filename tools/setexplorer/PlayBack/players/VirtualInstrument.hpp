/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include "../PlayBackSet.hpp"
#include "../audio/PlayBackFactory.hpp"

class VirtualInstrument
{
	const static uint8 c_maxOscillatorsPerSound = 24;

	struct OscillatorSources
	{
		UniquePointer<Audio::Source> low[c_maxOscillatorsPerSound];
		UniquePointer<Audio::Source> high[c_maxOscillatorsPerSound];
	};

public:
	//Constructor
	inline VirtualInstrument(const Sound::SoundData& soundData, const PlayBackSet& playBackSet, PlayBackFactory& playBackFactory) :
		soundData(soundData), playBackSet(playBackSet), playBackFactory(playBackFactory)
	{
	}

	//Methods
	bool CanDrumKeySound(const Pitch& pitch) const;
	bool CanMultiSampleSound(const Pitch& pitch) const;
	void NoteOff(const Pitch& pitch);

	//Inline
	inline bool CanKeySound(const Pitch& pitch) const
	{
		if(this->soundData.drumKitData.HasValue())
			return this->CanDrumKeySound(pitch);
		return this->CanMultiSampleSound(pitch);
	}

	inline void NoteOn(const Pitch& pitch, uint8 velocity)
	{
		if(this->soundData.drumKitData.HasValue())
			return this->NoteOnDrumKit(pitch, velocity);
		return this->NoteOnMultiSamples(pitch, velocity);
	}

private:
	//State
	const Sound::SoundData& soundData;
	const PlayBackSet& playBackSet;
	PlayBackFactory& playBackFactory;
	OscillatorSources keySources[128];

	//Methods
	bool CanOscillatorSound(const Pitch &pitch, const Sound::OSCMultiSampleSettings& osc) const;
	void NoteOnDrumKit(const Pitch& pitch, uint8 velocity);
	void NoteOnMultiSamples(const Pitch& pitch, uint8 velocity);
	void NoteOnOscillator(const Pitch& pitch, float32 gain, const Sound::OSCMultiSampleSettings& osc, UniquePointer<Audio::Source>& oscSource);
};
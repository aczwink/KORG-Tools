/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;
using namespace StdXX::Audio;
#include "VirtualInstrument.hpp"

class KeyboardRealtimePlayer
{
public:
	//Constructor
	inline KeyboardRealtimePlayer(const PlayBackSet& playBackSet, PlayBackFactory& playBackFactory) : playBackSet(playBackSet), playBackFactory(playBackFactory)
	{
	}

	//Inline
	inline bool CanKeySound(const Pitch& pitch) const
	{
		const auto& track = this->tracks[(uint8)KeyboardTrackNumber::Upper1];
		if(track.IsNull())
			return false;
		return track->CanKeySound(pitch);
	}

	inline void KeyDown(const Pitch& pitch, uint8 velocity)
	{
		this->tracks[(uint8)KeyboardTrackNumber::Upper1]->NoteOn(pitch, velocity);
	}

	inline void KeyUp(const Pitch& pitch)
	{
		this->tracks[(uint8)KeyboardTrackNumber::Upper1]->NoteOff(pitch);
	}

	inline void SelectSound(KeyboardTrackNumber trackNumber, const Sound::SoundData& soundData)
	{
		this->tracks[(uint8)trackNumber] = new VirtualInstrument(soundData, this->playBackSet, this->playBackFactory);
	}

private:
	//State
	const PlayBackSet& playBackSet;
	PlayBackFactory& playBackFactory;
	UniquePointer<VirtualInstrument> tracks[8];
};
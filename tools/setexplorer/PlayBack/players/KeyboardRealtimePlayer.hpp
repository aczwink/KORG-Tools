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
		for (bool &i : this->muted)
			i = false;
	}

	//Inline
	inline bool CanKeySound(const Pitch& pitch) const
	{
		for(uint8 i = 0; i < 4; i++)
		{
			const auto& track = this->tracks[(uint8)i];
			if(track.IsNull())
				continue;
			if(track->CanKeySound(pitch))
				return true;
		}
		return false;
	}

	inline bool IsTrackMuted(KeyboardTrackNumber trackNumber) const
	{
		return this->muted[(uint8)trackNumber];
	}

	inline void KeyDown(const Pitch& pitch, uint8 velocity)
	{
		for(uint8 i = 0; i < 4; i++)
		{
			auto& track = this->tracks[(uint8)i];
			if(track.IsNull())
				continue;
			if(this->muted[i])
				continue;
			track->NoteOn(pitch, velocity);
		}
	}

	inline void KeyUp(const Pitch& pitch)
	{
		for(uint8 i = 0; i < 4; i++)
		{
			auto& track = this->tracks[(uint8)i];
			if(track.IsNull())
				continue;
			if(this->muted[i])
				continue;
			track->NoteOff(pitch);
		}
	}

	inline void MuteToggleTrack(KeyboardTrackNumber trackNumber)
	{
		uint8 idx = static_cast<uint8>(trackNumber);
		this->muted[idx] = !this->muted[idx];
	}

	inline void SelectSound(KeyboardTrackNumber trackNumber, const Sound::SoundData& soundData)
	{
		this->tracks[(uint8)trackNumber] = new VirtualInstrument(soundData, this->playBackSet, this->playBackFactory);
	}

private:
	//State
	const PlayBackSet& playBackSet;
	PlayBackFactory& playBackFactory;
	UniquePointer<VirtualInstrument> tracks[4];
	bool muted[4];
};
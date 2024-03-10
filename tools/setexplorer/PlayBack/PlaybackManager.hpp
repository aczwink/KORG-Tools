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
#include "PlayBackSet.hpp"
#include "audio/SamplesManager.hpp"
#include "players/SingleSamplePlayer.hpp"
#include "players/KeyboardRealtimePlayer.hpp"
#include "players/StylePlayer.hpp"

class PlaybackManager
{
public:
	//Constructor
	inline PlaybackManager(const Set& set) : playBackSet(set)
	{
		DeviceEnumerator deviceEnumerator(DeviceType::Audio);
		this->audioDevice = deviceEnumerator.GetNextDevice().MoveCast<AudioDevice>();
		this->deviceContext = this->audioDevice->CreateDeviceContext();

		this->samplesManager = new SamplesManager(*this->deviceContext, this->playBackSet);
		this->singleSamplePlayer = new SingleSamplePlayer(*this->deviceContext);
		this->keyboardRealtimePlayer = new KeyboardRealtimePlayer(this->playBackSet, *this->samplesManager);
		this->stylePlayer = new StylePlayer(this->playBackSet, *this->samplesManager);
	}

	//Inline
	inline bool CanKeySound(const Pitch& pitch) const
	{
		return this->keyboardRealtimePlayer->CanKeySound(pitch);
	}

	inline void LoadStyle(const Style::StyleData &data, uint8 bpm)
	{
		this->stylePlayer->LoadStyle(data, bpm);
	}

	inline bool IsTrackMuted(KeyboardTrackNumber trackNumber) const
	{
		return this->keyboardRealtimePlayer->IsTrackMuted(trackNumber);
	}

	inline void KeyDown(const Pitch& pitch, uint8 velocity)
	{
		this->keyboardRealtimePlayer->KeyDown(pitch, velocity);
	}

	inline void KeyUp(const Pitch& pitch)
	{
		this->keyboardRealtimePlayer->KeyUp(pitch);
	}

	inline void MuteToggleTrack(KeyboardTrackNumber trackNumber)
	{
		this->keyboardRealtimePlayer->MuteToggleTrack(trackNumber);
	}

	inline void PlaySample(uint64 sampleId)
	{
		auto buffer = this->samplesManager->LoadSample(sampleId);

		this->singleSamplePlayer->PlayBuffer(buffer);
	}

	inline void SelectSound(AccompanimentTrackNumber keyboardTrackNumber, const Sound::SoundData& soundData)
	{
		this->stylePlayer->SelectSound(keyboardTrackNumber, soundData);
	}

	inline void SelectSound(KeyboardTrackNumber keyboardTrackNumber, const Sound::SoundData& soundData)
	{
		this->keyboardRealtimePlayer->SelectSound(keyboardTrackNumber, soundData);
	}

	inline void ToggleStartStopStyle()
	{
		this->stylePlayer->ToggleStartStopStyle();
	}

private:
	//State
	AutoPointer<AudioDevice> audioDevice;
	UniquePointer<Audio::DeviceContext> deviceContext;
	PlayBackSet playBackSet;
	UniquePointer<SamplesManager> samplesManager;
	UniquePointer<SingleSamplePlayer> singleSamplePlayer;
	UniquePointer<KeyboardRealtimePlayer> keyboardRealtimePlayer;
	UniquePointer<StylePlayer> stylePlayer;
};
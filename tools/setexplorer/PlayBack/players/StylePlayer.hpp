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
#include "VirtualInstrument.hpp"

class StylePlayer : private MIDI::OutputSequencer
{
public:
	//Constructor
	inline StylePlayer(const PlayBackSet& playBackSet, PlayBackFactory& playBackFactory) : playBackSet(playBackSet), playBackFactory(playBackFactory)
	{
		this->tempo = 100;
	}

	//Inline
	inline bool IsPlaying()
	{
		return !this->sequencerThread.IsNull() and this->sequencerThread->IsAlive();
	}

	inline void LoadStyle(const Style::StyleData& data, uint8 bpm)
	{
		if(!this->sequencerThread.IsNull())
			this->Stop();

		StandardMIDIFormatConverter loader(bpm);
		this->loadedProgram = loader.LoadVariation(0, 0, data);
		this->tempo = bpm;
	}

	inline void SelectSound(AccompanimentTrackNumber trackNumber, const Sound::SoundData& soundData)
	{
		this->tracks[(uint8)trackNumber] = new VirtualInstrument(soundData, this->playBackSet, this->playBackFactory);
	}

	inline void ToggleStartStopStyle()
	{
		if(this->IsPlaying())
			this->Stop();
		else
			this->Start();
	}

	//Methods
	void Start();
	void Stop();

private:
	//Methods
	void ControlChange(uint32 trackNumber, uint8 controllerNumber, uint8 controllerValue) override
	{
		//TODO: implement me
	}

	void NoteOff(uint32 trackNumber, uint8 note, uint8 velocity) override
	{
		auto& track = this->tracks[trackNumber];
		if(track.IsNull())
			return;

		Pitch pitch(note);
		track->NoteOff(pitch);
	}

	void NoteOn(uint32 trackNumber, uint8 note, uint8 velocity) override
	{
		auto& track = this->tracks[trackNumber];
		if(track.IsNull())
			return;

		Pitch pitch(note);
		track->NoteOn(pitch, velocity);
	}

	void ProgramChange(uint32 trackNumber, uint8 programNumber) override
	{
		//TODO: implement me
	}

private:
	//State
	const PlayBackSet& playBackSet;
	PlayBackFactory& playBackFactory;
	UniquePointer<VirtualInstrument> tracks[8];
	Optional<MIDI::Program> loadedProgram;
	uint8 tempo;
	UniquePointer<Thread> sequencerThread;
	bool looping;
};
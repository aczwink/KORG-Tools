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
#include <libkorg/DataViews/StyleView.hpp>

namespace libKORG
{
	//Converts to General MIDI https://en.wikipedia.org/wiki/General_MIDI
	class StandardMIDIFormatConverter
	{
	public:
		//Constructor
		inline StandardMIDIFormatConverter(uint16 beatsPerMinute)
		{
			this->beatsPerMinute = beatsPerMinute;
			this->endTime = 0;
		}

		//Methods
		StdXX::MIDI::Program LoadVariation(uint8 variation, uint8 chordVariation, const Style::StyleData& data);

	private:
		//State
		uint16 beatsPerMinute;
		uint64 endTime;

		//Methods
		void AddMetaEvent(const Style::KORG_MIDI_Event& event, uint64 scaledTime, StdXX::MIDI::Program& program);
		void AddTempoMessage(uint16 bpm, StdXX::MIDI::Program& program) const;
		void AddTimeSignatureMessage(StdXX::MIDI::Program& program) const;
		uint8 FindMostPreciseFrameRate(const IChordVariationView& chordVariationView) const;
		void LoadTrackEvents(AccompanimentTrackNumber accompanimentTrackNumber, const IStyleElementView& styleElementView, uint8 chordVariation, StdXX::MIDI::Program& program, uint8 targetFPS);
		uint8 MapTrackNumberToGeneralMIDIChannel(AccompanimentTrackNumber accompanimentTrackNumber) const;
	};
}
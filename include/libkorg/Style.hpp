/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "BankObject.hpp"
#include "ProgramChangeSequence.hpp"

namespace libKORG
{
	enum class KORG_MIDI_EventType
	{
		NoteOff = 0,
		NoteOn = 1,
		ControlChange = 3,
		Aftertouch = 5,
		Bend = 6,
		RXnoiseOff = 12,
		RXnoiseOn = 13,
		EndOfTrack = 0x92F,
		Unknown = 0x97E,
		DeltaTime
	};

	struct KORG_MIDI_Event
	{
		KORG_MIDI_EventType type;
		uint16 value1;
		uint8 value2;
	};

	enum class OctavePitch
	{
		C = 0,
		C_SHARP = 1,
		D = 2,
		D_SHARP = 3,
		E = 4,
		F = 5,
		F_SHARP = 6,
		G = 7,
		G_SHARP = 8,
		A = 9,
		A_SHARP = 10,
		B = 11,
	};

	struct Pitch
	{
		int8 octave;
		OctavePitch pitchWithinOctave;

		inline Pitch()
		{
			this->octave = 0;
			this->pitchWithinOctave = OctavePitch::C;
		}

		inline Pitch(uint8 encoded)
		{
			this->octave = encoded / 12;
			this->pitchWithinOctave = static_cast<OctavePitch>(encoded % 12);
		}

		inline uint8 Encode() const
		{
			return this->octave * 12 + (uint8)this->pitchWithinOctave;
		}
	};

	struct StyleTrackData
	{
		uint8 expression;
		StdXX::UniquePointer<ProgramChangeSequence> soundProgramChangeSeq;
		Pitch keyboardRangeBottom;
		Pitch keyboardRangeTop;
	};

	class Style : public BankObject
	{
	public:
		//Methods
		void WriteData(StdXX::DataWriter &dataWriter) const override;
	};
}
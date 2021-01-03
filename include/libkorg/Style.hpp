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
		Ctrl = 3,
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

	enum class Pitch
	{
		C_MINUS_1 = 0,
		//TODO: rest ...
		A0 = 21,
		//TODO: rest ...
		E1 = 28,
		//TODO: rest ...
		B1 = 35,
		//TODO: rest ...
		F_SHARP_2 = 42,
		G2 = 43,
		//TODO: rest ...
		G4 = 67,
		//TODO: rest ...
		C8 = 108,
		//TODO: rest ...
		D_SHARP_9 = 123,
		//TODO: rest ...
		G9 = 127
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
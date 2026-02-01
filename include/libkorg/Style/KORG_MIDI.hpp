/*
 * Copyright (c) 2020-2026 Amir Czwink (amir130@hotmail.de)
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

namespace libKORG::Style
{
	enum class KORG_MIDI_EventType
	{
		NoteOff = 0,
		NoteOn = 1,
		ControlChange = 3,
		Aftertouch = 5,
		Bend = 6,
		MetaEvent = 9,
		RXnoiseOff = 12,
		RXnoiseOn = 13,
		DeltaTime
	};

	enum class KORG_MIDI_MetaEventType
	{
		EndOfTrack = 0x2F,
		UnknownMaster = 0x7E,
	};

	struct KORG_MIDI_Event
	{
		KORG_MIDI_EventType type;
		uint16 value1;
		uint8 value2;
		StdXX::Optional<uint8> unknownAdditional;

		struct
		{
			KORG_MIDI_MetaEventType type;
			uint16 dataLength;
			byte data[4];
		} metaEvent;
	};

	struct MIDI_Track
	{
		enum
		{
			DrumOrPerc = 0x2000008,
			Bass = 0x3000008,
			Accompaniment = 0x4000008,
			Guitar = 0x5010008,
		} chunkType;

		uint8 timeScale;

		struct
		{
			uint8 unknown2;
		} drumOrPercData;

		struct
		{
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			uint8 unknown5;
		} bassData;

		struct
		{
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
		} accompanimentData;

		struct
		{
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
		} guitarData;

		StdXX::DynamicArray<KORG_MIDI_Event> events;
	};

	struct MasterMIDI_Track
	{
		uint8 unknown1;
		uint8 unknown2;
		uint8 unknown3;

		StdXX::DynamicArray<KORG_MIDI_Event> events;
	};
}
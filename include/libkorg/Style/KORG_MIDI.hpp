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

namespace libKORG::Style
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
		StdXX::DynamicArray<uint8> additional9Bytes;
	};

	struct MIDI_Track
	{
		enum
		{
			CHUNK_0x2000008 = 0x2000008,
			CHUNK_0x3000008 = 0x3000008,
			CHUNK_0x4000008 = 0x4000008,
			CHUNK_0x5010008 = 0x5010008,
		} chunkType;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
		} _0x2000008_data;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			uint8 unknown5;
		} _0x3000008_data;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
		} _0x4000008_data;

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
		} _0x5010008_data;

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
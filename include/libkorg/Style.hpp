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

	struct MIDI_Track
	{
		enum
		{
			CHUNK_0x2000008,
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

	struct GeneralStyleElementData
	{
		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			byte unknownChordTable[0x18];
			uint8 unknown3;
		} _0x1010108_chunk;

		StyleTrackData styleTrackData[8];

		struct
		{
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			uint8 unknown5;
			uint8 unknown6;
			uint8 unknown7;
			uint8 unknown8;
			uint8 unknown9;
			uint8 unknown10;
			uint8 unknown11;
			uint8 unknown12;
			uint8 unknown13;
			uint8 unknown14;
			uint8 unknown15;
			uint8 unknown16;
			uint8 unknown17;
			uint8 unknown18;
			uint8 unknown19;
			uint8 unknown20[16];
		} _0x2000308_chunk;
	};

	struct ChordVariationData
	{
		MIDI_Track midiTrack;
		StdXX::DynamicArray<uint16> unknown;
	};

	struct VariationStyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[6];
	};

	struct StyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[2];
	};

	struct StyleData
	{
		struct
		{
			StdXX::String name;
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			uint8 unknown5;
			uint8 unknown6;
			uint8 unknown7;
			uint8 unknown8;
			uint8 unknown9;
			uint8 unknown10;
			uint8 unknown11;
			uint8 unknown12;
			uint8 unknown13;
			uint8 unknown14;
		} _0x1000308_chunk;

		struct
		{
			uint16 highest;
			StdXX::DynamicArray<uint16> values;
		} _0x1000008_chunk;

		StdXX::StaticArray<MIDI_Track, 169> midiTracks;

		VariationStyleElementData variation[4];

		StyleElementData styleElements[11];
	};

	class Style : public BankObject
	{
	public:
		//Constructor
		inline Style(StyleData&& data) : data(StdXX::Move(data))
		{
		}

		//Methods
		void WriteData(StdXX::DataWriter &dataWriter) const override;

		//Properties
		inline const StyleData& Data() const
		{
			return this->data;
		}

	private:
		//Members
		StyleData data;
	};
}
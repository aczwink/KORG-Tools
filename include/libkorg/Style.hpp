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
#include "UnknownChunk.hpp"

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
			this->octave--; //lowest note is 0 => C-1
			this->pitchWithinOctave = static_cast<OctavePitch>(encoded % 12);
		}

		inline uint8 Encode() const
		{
			return (this->octave + 1) * 12 + (uint8)this->pitchWithinOctave;
		}
	};

	struct StyleTrackData
	{
		uint8 expression;
		StdXX::UniquePointer<ProgramChangeSequence> soundProgramChangeSeq;
		Pitch keyboardRangeBottom;
		Pitch keyboardRangeTop;

		//Constructor
		StyleTrackData() = default;

		inline StyleTrackData(const StyleTrackData& styleTrackData)
		{
			*this = styleTrackData;
		}

		//Operators
		inline StyleTrackData& operator=(const StyleTrackData& styleTrackData)
		{
			this->expression = styleTrackData.expression;
			this->soundProgramChangeSeq = new ProgramChangeSequence(*styleTrackData.soundProgramChangeSeq);
			this->keyboardRangeBottom = styleTrackData.keyboardRangeBottom;
			this->keyboardRangeTop = styleTrackData.keyboardRangeTop;

			return *this;
		}
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

	struct GeneralStyleElementData
	{
		libKORG::UnknownChunk unknownChordTable;
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

		//Constructors
		GeneralStyleElementData() = default;

		//Operators
		inline void CopyFrom(const GeneralStyleElementData& styleElementData)
		{
			this->unknownChordTable = styleElementData.unknownChordTable;
			for(uint8 i = 0; i < 8; i++)
				this->styleTrackData[i] = styleElementData.styleTrackData[i];
			this->_0x2000308_chunk = styleElementData._0x2000308_chunk;
		}
	};

	struct ChordVariationData
	{
		MIDI_Track midiTrack;
		StdXX::DynamicArray<uint16> unknown;
	};

	struct VariationStyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[6];

		//Constructors
		VariationStyleElementData() = default;

		//Operators
		inline VariationStyleElementData& operator=(const VariationStyleElementData& styleElementData)
		{
			this->CopyFrom(styleElementData);
			for(uint8 i = 0; i < 6; i++)
				this->cv[i] = styleElementData.cv[i];
			return *this;
		}
	};

	struct StyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[2];

		//Operators
		inline StyleElementData& operator=(const StyleElementData& styleElementData)
		{
			this->CopyFrom(styleElementData);
			for(uint8 i = 0; i < 2; i++)
				this->cv[i] = styleElementData.cv[i];
			return *this;
		}
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

		StdXX::DynamicArray<MIDI_Track> midiTracks;
		VariationStyleElementData variation[4];
		StyleElementData styleElements[11];

		//Constructors
		StyleData() = default;

		inline StyleData(const StyleData& styleData)
		{
			this->_0x1000308_chunk = styleData._0x1000308_chunk;
			this->_0x1000008_chunk = styleData._0x1000008_chunk;
			this->midiTracks = styleData.midiTracks;
			for(uint8 i = 0; i < 4; i++)
				this->variation[i] = styleData.variation[i];
			for(uint8 i = 0; i < 11; i++)
				this->styleElements[i] = styleData.styleElements[i];
		}

		StyleData(StyleData&& styleData) = default;
	};

	class Style : public BankObject
	{
	public:
		//Constructors
		inline Style(StyleData&& data) : data(StdXX::Move(data))
		{
		}

		inline Style(const Style& style) : data(style.data)
		{
		}

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
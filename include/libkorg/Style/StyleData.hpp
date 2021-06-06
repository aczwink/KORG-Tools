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
#include "StyleElementData.hpp"

namespace libKORG::Style
{
	struct StyleData
	{
		struct
		{
			StdXX::String name;
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint16 enabledStyleElements;
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
		} _0x1000308_chunk;

		StdXX::DynamicArray<uint16> oneBasedMIDITrackMappingIndices;
		StdXX::DynamicArray<MIDI_Track> midiTracks;
		VariationStyleElementData variation[4];
		StyleElementData styleElements[11];

		//Constructors
		StyleData() = default;
		StyleData(const StyleData& styleData) = default;
		StyleData(StyleData&& styleData) = default;
	};
}
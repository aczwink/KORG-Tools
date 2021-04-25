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
//Local
#include <libkorg/Numbers.hpp>
#include "KORG_MIDI.hpp"

namespace libKORG::Style
{
	enum class ChordVariationTrackType
	{
		DrumOrPerc = 0,
		Bass = 1,
		Accompany = 2,
	};

	struct ChordVariationTrackMapping
	{
		ChordVariationTrackType type;
		AccompanimentTrackNumber trackNumber;
	};

	struct ChordVariationData
	{
		MasterMIDI_Track masterMidiTrack;
		StdXX::DynamicArray<ChordVariationTrackMapping> trackMapping;
	};
}
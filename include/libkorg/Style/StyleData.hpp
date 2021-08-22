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
#include "StyleElementData.hpp"
#include "StyleInfoData.hpp"
#include "StyleElementNumber.hpp"

namespace libKORG::Style
{
	struct StyleData
	{
		StyleInfoData styleInfoData;
		StdXX::DynamicArray<uint16> oneBasedMIDITrackMappingIndices;
		StdXX::DynamicArray<MIDI_Track> midiTracks;
		VariationStyleElementData variation[4];
		StyleElementData styleElements[11];

		//Constructors
		inline StyleData()
		{
			this->styleElements[(uint8)StyleElementNumber::Break].styleElementInfoData.cueMode = CueMode::EnterImmediately_ContinueWithCurrentMeasure;

			this->styleElements[(uint8)StyleElementNumber::Ending1].styleElementInfoData.cueMode = CueMode::EnterOnNextMeasure_ContinueWithFirstMeasure;
			this->styleElements[(uint8)StyleElementNumber::Ending2].styleElementInfoData.cueMode = CueMode::EnterOnNextMeasure_ContinueWithFirstMeasure;
			this->styleElements[(uint8)StyleElementNumber::Ending3].styleElementInfoData.cueMode = CueMode::EnterImmediately_ContinueWithFirstMeasure;

			this->styleElements[(uint8)StyleElementNumber::Fill1].styleElementInfoData.cueMode = CueMode::EnterImmediately_ContinueWithCurrentMeasure;
			this->styleElements[(uint8)StyleElementNumber::Fill2].styleElementInfoData.cueMode = CueMode::EnterImmediately_ContinueWithCurrentMeasure;
			this->styleElements[(uint8)StyleElementNumber::Fill3].styleElementInfoData.cueMode = CueMode::EnterImmediately_ContinueWithCurrentMeasure;
			this->styleElements[(uint8)StyleElementNumber::Fill4].styleElementInfoData.cueMode = CueMode::EnterImmediately_ContinueWithCurrentMeasure;

			this->styleElements[(uint8)StyleElementNumber::Intro1].styleElementInfoData.cueMode = CueMode::EnterOnNextMeasure_ContinueWithFirstMeasure;
			this->styleElements[(uint8)StyleElementNumber::Intro2].styleElementInfoData.cueMode = CueMode::EnterOnNextMeasure_ContinueWithFirstMeasure;
			this->styleElements[(uint8)StyleElementNumber::Intro3].styleElementInfoData.cueMode = CueMode::EnterOnNextMeasure_ContinueWithFirstMeasure;
		}

		StyleData(const StyleData& styleData) = default;
		StyleData(StyleData&& styleData) = default;
	};
}
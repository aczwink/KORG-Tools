/*
 * Copyright (c) 2021-2026 Amir Czwink (amir130@hotmail.de)
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
#include "TrackView.hpp"

class ChordVariationView : public libKORG::IChordVariationView
{
public:
	//Constructor
	inline ChordVariationView(const libKORG::Style::StyleData &styleData, const libKORG::Style::ChordVariationData& cv, uint8 trackBaseIndex) : masterMidiTrack(cv.masterMidiTrack), trackViews(cv.trackMapping.GetNumberOfElements())
	{
		for(uint8 i = 0; i < cv.trackMapping.GetNumberOfElements(); i++)
		{
			uint8 trackMappingIndex = trackBaseIndex + i;
			uint8 trackIndex = styleData.oneBasedMIDITrackMappingIndices[trackMappingIndex] - 1;
			libKORG::AccompanimentTrackNumber trackNumber = cv.trackMapping[i].trackNumber;

			this->trackViews[i] = new TrackView(styleData, trackNumber, trackIndex);
		}
	}

	//Public methods
	bool DoesHaveAnyData() const override
	{
		for(const auto& track : this->trackViews)
		{
			if(!track->IsEmpty())
				return true;
		}
		return false;
	}

	const libKORG::Style::MasterMIDI_Track& GetMasterMIDITrack() const override
	{
		return this->masterMidiTrack;
	}

	const libKORG::ITrackView &GetTrack(uint8 trackIndex) const override
	{
		return *this->trackViews[trackIndex];
	}

	uint8 GetTrackCount() const override
	{
		return this->trackViews.GetNumberOfElements();
	}

private:
	//State
	const libKORG::Style::MasterMIDI_Track& masterMidiTrack;
	StdXX::FixedArray<StdXX::UniquePointer<libKORG::ITrackView>> trackViews;
};
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
#include "EmptyTrackView.hpp"

class ChordVariationView : public libKORG::IChordVariationView
{
public:
	//Constructor
	inline ChordVariationView(const libKORG::Style::StyleData &styleData, const libKORG::Style::ChordVariationData& cv, uint8 trackBaseIndex)
	{
		//TODO: accompaniment tracks can actually have multiple midi tracks assigned. apparently the midi master track coordinates when which track is played.
		//TODO: the current implementation of this class can not satisfy this. it will always just take the last midi track assigned to a accompaniment track
		for(uint8 i = 0; i < cv.trackMapping.GetNumberOfElements(); i++)
		{
			uint8 trackMappingIndex = trackBaseIndex + i;
			uint8 trackIndex = styleData.oneBasedMIDITrackMappingIndices[trackMappingIndex] - 1;
			auto trackNumber = cv.trackMapping[i].trackNumber;

			this->trackViews[(uint8)trackNumber] = new TrackView(styleData, trackIndex);
		}
		for(const auto& trackNumber : libKORG::AccompanimentTrackNumbers)
		{
			uint8 index = (uint8) trackNumber;
			if(this->trackViews[index].IsNull())
				this->trackViews[index] = new EmptyTrackView();
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

	const libKORG::ITrackView &GetTrack(libKORG::AccompanimentTrackNumber trackNumber) const override
	{
		return *this->trackViews[static_cast<uint32>(trackNumber)];
	}

private:
	//State
	StdXX::StaticArray<StdXX::UniquePointer<libKORG::ITrackView>, 8> trackViews;
};
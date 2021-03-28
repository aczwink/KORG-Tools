/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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

namespace libKORG
{
	class SoundMapper
	{
	public:
		//Constructor
		SoundMapper(StdXX::Map<ProgramChangeSequence, ProgramChangeSequence>&& soundMappings) : soundMappings(StdXX::Move(soundMappings))
		{
		}

		//Methods
		void Map(TrackProperties& trackProperties) const
		{
			this->MapSound(trackProperties.soundProgramChangeSeq);
		}

		void Map(Style& style) const
		{
			for(VariationStyleElementData& variationStyleElementData : style.data.variation)
			{
				for(StyleTrackData& styleTrackData : variationStyleElementData.styleTrackData)
				{
					this->Map(styleTrackData);
				}
			}

			for(StyleElementData& styleElementData : style.data.styleElements)
			{
				for(StyleTrackData& styleTrackData : styleElementData.styleTrackData)
				{
					this->Map(styleTrackData);
				}
			}
		}

		void Map(SingleTouchSettings& sts) const
		{
			for(TrackProperties& trackProperties : sts.accompanimentSettings.trackProperties)
			{
				this->Map(trackProperties);
			}

			for(KeyboardSettings& keyboardSettings : sts.keyboardSettings)
			{
				for(TrackProperties& trackProperties : keyboardSettings.trackProperties)
				{
					this->Map(trackProperties);
				}
			}
		}

	private:
		//Members
		StdXX::Map<ProgramChangeSequence, ProgramChangeSequence> soundMappings;

		//Methods
		void Map(StyleTrackData& styleTrackData) const
		{
			this->MapSound(*styleTrackData.soundProgramChangeSeq);
		}

		void MapSound(ProgramChangeSequence& programChangeSequence) const
		{
			if(this->soundMappings.Contains(programChangeSequence))
				programChangeSequence = this->soundMappings.Get(programChangeSequence);
		}
	};
}
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

#include <libkorg/Set.hpp>

namespace libKORG
{
	class PerformanceV2ToV1Converter
	{
	public:
		//Constructor
		inline PerformanceV2ToV1Converter(const Set& targetSet) : baseSTS(this->FindFirstSTS(targetSet))
		{
		}

		//Methods
		SingleTouchSettings Convert(const SingleTouchSettings& source) const
		{
			SingleTouchSettings result = this->baseSTS;
			for(uint8 i = 0; i < 4; i++)
				this->MapKeyboardSettings(result.keyboardSettings[i], source.keyboardSettings[i]);
			this->MapAccompanimentSettings(result.accompanimentSettings, source.accompanimentSettings);

			return result;
		}

	private:
		//Members
		const SingleTouchSettings& baseSTS;

		//Methods
		const SingleTouchSettings& FindFirstSTS(const Set& set)
		{
			for(const auto& bankEntry : set.StyleBanks())
			{
				for(const auto& objectEntry : bankEntry.value.Objects())
				{
					return objectEntry.value.Get<1>()->STS();
				}
			}

			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		void MapAccompanimentSettings(AccompanimentSettings& target, const AccompanimentSettings& source) const
		{
			for(uint8 i = 0; i < 8; i++)
				this->MapTrackProperties(target.trackProperties[i], source.trackProperties[i]);
		}

		void MapKeyboardSettings(KeyboardSettings& target, const KeyboardSettings& source) const
		{
			target.name = source.name;
			for(uint8 i = 0; i < 4; i++)
				this->MapTrackProperties(target.trackProperties[i], source.trackProperties[i]);
		}

		void MapTrackProperties(TrackProperties& target, const TrackProperties& source) const
		{
			target.soundProgramChangeSeq = source.soundProgramChangeSeq;
			target.highGainTimes2 = source.highGainTimes2;
			target.midGainTimes2 = source.midGainTimes2;
			target.lowGainTimes2 = source.lowGainTimes2;
			target.pbSensitivity = source.pbSensitivity;
			target.dry = source.dry;
			target.fxMaster1 = source.fxMaster1;
			target.fxMaster2 = source.fxMaster2;
			target.octaveTranspose = source.octaveTranspose;
			target.detune = source.detune;
			target.pan = source.pan;
			target.volume = source.volume;
		}
	};
}
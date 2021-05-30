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
			this->Convert(source.V2Data(), result.V1Data());
			return result;
		}

	private:
		//Members
		const SingleTouchSettings& baseSTS;

		//Methods
		void Convert(const Performance::V2::STSData& source, Performance::V1::STSData& target) const
		{
			this->ConvertGeneralData(source, target);

			for(uint8 i = 0; i < 4; i++)
				this->MapKeyboardSettings(target.keyboardSettings[i], source.keyboardSettings[i]);
		}

		void ConvertGeneralData(const Performance::V2::GeneralData& source, Performance::V1::GeneralData& target) const
		{
			this->Map04Chunk(source._0x04020008_data, target._0x04000108_data);

			this->MapAccompanimentSettings(target.accompanimentSettings, source.accompanimentSettings);
		}

		const SingleTouchSettings& FindFirstSTS(const Set& set)
		{
			for(const auto& bankEntry : set.styleBanks.Entries())
			{
				for(const auto& objectEntry : bankEntry.bank.Objects())
				{
					return objectEntry.object->STS();
				}
			}

			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		void Map04Chunk(const Performance::V2::_0x04020008_chunk& source, Performance::V1::_0x04000108_chunk& target) const
		{
			target.metronomeTempo = source.metronomeTempo;
		}

		void MapAccompanimentSettings(Performance::V1::AccompanimentSettings& target, const Performance::V2::AccompanimentSettings& source) const
		{
			for(uint8 i = 0; i < 8; i++)
				this->MapTrackProperties(target.trackSettings[i], source.trackSettings[i]);
		}

		void MapKeyboardSettings(Performance::V1::KeyboardSettings& target, const Performance::V2::KeyboardSettings& source) const
		{
			target.name = source.name;
			for(uint8 i = 0; i < 4; i++)
				this->MapTrackProperties(target.trackSettings[i], source.trackSettings[i]);
		}

		void MapTrackProperties(Performance::V1::TrackSettings& target, const Performance::V2::TrackSettings& source) const
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
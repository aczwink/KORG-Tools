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
		//Methods
		SingleTouchSettings Convert(const SingleTouchSettings& source) const
		{
			StdXX::UniquePointer<Performance::V1::STSData> stsData = new Performance::V1::STSData;
			this->Convert(source.V2Data(), *stsData);
			return stsData;
		}

	private:
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
			target._0x1B000008_data = source._0x1B000008_data;
			target._0x1A000008_data = source._0x1A000008_data;
		}

		void Map04Chunk(const Performance::V2::_0x04020008_chunk& source, Performance::V1::_0x04000108_chunk& target) const
		{
			target.metronomeTempo = source.metronomeTempo;
			//TODO: check whats compatible here
		}

		void MapAccompanimentSettings(Performance::V1::AccompanimentSettings& target, const Performance::V2::AccompanimentSettings& source) const
		{
			target._0x06000008_data = source._0x06000008_data;
			target.padSettings = source.padSettings;

			for(uint8 i = 0; i < 8; i++)
				this->MapTrackProperties(target.trackSettings[i], source.trackSettings[i]);
		}

		void MapKeyboardSettings(Performance::V1::KeyboardSettings& target, const Performance::V2::KeyboardSettings& source) const
		{
			target.name = source.name;
			target.unknown1 = source.unknown1;
			target._0x10000008_data = source._0x10000008_data;
			target._0x11000008_data = source._0x11000008_data;
			target._0x12000108_data = source._0x12000108_data;
			target.scaleSettings = source.scaleSettings;
			target._0x14000008_data = source._0x14000008_data;
			target._0x15000108_data = source._0x15000108_data;

			//TODO: fx

			for(uint8 i = 0; i < 4; i++)
				this->MapTrackProperties(target.trackSettings[i], source.trackSettings[i]);

			target._0x18000008_data = source._0x18000008_data;
		}

		void MapTrackProperties(Performance::V1::TrackSettings& target, const Performance::V2::TrackSettings& source) const
		{
			target = source; //slicing is desired here
		}
	};
}
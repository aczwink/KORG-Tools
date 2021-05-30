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
	class PerformanceV0ToV1Converter
	{
	public:
		//Methods
		Performance::V1::PerformanceData Convert(const Performance::V0::PerformanceData& source) const
		{
			Performance::V1::PerformanceData target;

			this->MapKeyboardSettings(source.keyboardSettings, target.keyboardSettings);

			return target;
		}

	private:
		//Methods
		void MapKeyboardSettings(const Performance::V0::KeyboardSettings& source, Performance::V1::KeyboardSettings& target) const
		{
			for(uint32 i = 0; i < target.trackSettings.Size(); i++)
			{
				target.trackSettings[i].soundProgramChangeSeq = source.trackSettings[i].soundProgramChangeSeq;
			}
			target.scaleSettings = source.scaleSettings;
		}
	};
}
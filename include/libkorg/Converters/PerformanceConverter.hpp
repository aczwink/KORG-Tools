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

#include "PerformanceV0ToV1Converter.hpp"

namespace libKORG
{
	class PerformanceConverter
	{
	public:
		//Methods
		PerformanceObject Convert(const PerformanceObject& source, uint8 majorVersion) const
		{
			switch(source.Version())
			{
				case 0:
					return this->Convert(source.V0Data(), majorVersion);
				case 1:
					return this->Convert(source.V1Data(), majorVersion);
				case 2:
					return this->Convert(source.V2Data(), majorVersion);
				default:
					RAISE(StdXX::ErrorHandling::IllegalCodePathError);
			}
		}

	private:
		//Methods
		PerformanceObject Convert(const Performance::V0::PerformanceData& performanceData, uint8 majorVersion) const
		{
			switch(majorVersion)
			{
				case 0:
					return PerformanceObject(new Performance::V0::PerformanceData(performanceData));
				case 1:
				{
					PerformanceV0ToV1Converter converter;
					return PerformanceObject(new Performance::V1::PerformanceData(converter.Convert(performanceData)));
				}
				case 2:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				default:
					RAISE(StdXX::ErrorHandling::IllegalCodePathError);
			}
		}

		PerformanceObject Convert(const Performance::V1::PerformanceData& performanceData, uint8 majorVersion) const
		{
			switch(majorVersion)
			{
				case 0:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				case 1:
					return PerformanceObject(new Performance::V1::PerformanceData(performanceData));
				case 2:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				default:
					RAISE(StdXX::ErrorHandling::IllegalCodePathError);
			}
		}

		PerformanceObject Convert(const Performance::V2::PerformanceData& performanceData, uint8 majorVersion) const
		{
			switch(majorVersion)
			{
				case 0:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				case 1:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
				case 2:
					return PerformanceObject(new Performance::V2::PerformanceData(performanceData));
				default:
					RAISE(StdXX::ErrorHandling::IllegalCodePathError);
			}
		}
	};
}
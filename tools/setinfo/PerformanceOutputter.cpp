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
//Class header
#include "PerformanceOutputter.hpp"
//Local
#include "PerformanceV1Outputter.hpp"
#include "PerformanceV0Outputter.hpp"
#include "PerformanceV2Outputter.hpp"

//Public methods
void PerformanceOutputter::Output(const PerformanceObject &performanceObject)
{
	switch(performanceObject.Version())
	{
		case 0:
		{
			PerformanceV0Outputter performanceV0Outputter(this->formattedOutputter);
			performanceV0Outputter.Output(performanceObject.V0Data());
		}
		break;
		case 1:
		{
			PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);
			performanceV1Outputter.Output(performanceObject.V1Data());
		}
		break;
		case 2:
		{
			PerformanceV2Outputter performanceV2Outputter(this->formattedOutputter);
			performanceV2Outputter.Output(performanceObject.V2Data());
		}
		break;
	}
}

void PerformanceOutputter::Output(const SingleTouchSettings &sts)
{
	Section section(u8"Single Touch Settings", this->formattedOutputter);

	switch(sts.Version())
	{
		case 0:
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			//PerformanceV0Outputter performanceV0Outputter(this->formattedOutputter);
			//performanceV0Outputter.Output(sts.V0Data());
		}
		break;
		case 1:
		{
			PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);
			performanceV1Outputter.Output(sts.V1Data());
		}
			break;
		case 2:
		{
			PerformanceV2Outputter performanceV2Outputter(this->formattedOutputter);
			performanceV2Outputter.Output(sts.V2Data());
		}
			break;
	}
}

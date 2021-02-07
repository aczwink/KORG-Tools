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
//Class header
#include "StyleOutputter.hpp"
//Local
#include "PerformanceOutputter.hpp"

//Public methods
void StyleOutputter::Output(const FullStyle &fullStyle)
{
	this->Output(fullStyle.Style());

	PerformanceOutputter performanceOutputter(this->formattedOutputter);
	//performanceOutputter.Output(fullStyle.STS()); //TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

//Private methods
void StyleOutputter::Output(const Style &style)
{
	Section section(u8"Style Data", this->formattedOutputter);
	this->Output(style.Data());
}

void StyleOutputter::Output(const StyleData &styleData)
{
	{
		Section chunkSection(u8"0x1000008", this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"highest", styleData._0x1000008_chunk.highest);
		for(uint32 i = 0; i < styleData._0x1000008_chunk.values.GetNumberOfElements(); i++)
			this->formattedOutputter.OutputProperty(String::Number(i), styleData._0x1000008_chunk.values[i]);
	}
}

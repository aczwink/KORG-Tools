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
#include <libkorg/Converters/SampleRemover.hpp>
//Namespaces
using namespace libKORG;

void SampleRemover::RemoveSampleFromIndex(uint32 sampleIndex)
{
	auto& sampleEntries = this->data.sampleEntries;
	sampleEntries.Remove(sampleIndex);

	auto& drumSampleEntries = this->data.drumSampleEntries;
	for(uint32 i = 0; i < drumSampleEntries.GetNumberOfElements();)
	{
		if(drumSampleEntries[i].sampleIndexLeft == sampleIndex)
		{
			drumSampleEntries.Remove(i);
			continue;
		}
		if(drumSampleEntries[i].sampleIndexRight == sampleIndex)
			drumSampleEntries[i].sampleIndexRight = -1;

		if(drumSampleEntries[i].sampleIndexLeft > sampleIndex)
			drumSampleEntries[i].sampleIndexLeft--;
		if(drumSampleEntries[i].sampleIndexRight > sampleIndex)
			drumSampleEntries[i].sampleIndexRight--;

		i++;
	}

	auto& keyboardZones = this->data.keyboardZones;
	for(auto& zone : keyboardZones)
	{
		if(zone.sampleNumber == -1)
			continue;
		else if(zone.sampleNumber == sampleIndex)
			zone.sampleNumber = -1;
		else if(zone.sampleNumber > sampleIndex)
			zone.sampleNumber--;
	}
}
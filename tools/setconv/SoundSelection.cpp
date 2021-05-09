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
#include "SoundSelection.hpp"

//Constructor
SoundSelection::SoundSelection(const Set &set, const DynamicArray<const SoundObject *> &selection)
{
	for(const auto& sound : selection)
	{
		for(const auto& osc : sound->data.oscillators)
		{
			if(osc.high.source == Sound::MultiSampleSource::RAM)
				this->multiSampleIds.Insert(osc.high.multiSampleId);
			if(osc.low.source == Sound::MultiSampleSource::RAM)
				this->multiSampleIds.Insert(osc.low.multiSampleId);
		}
	}

	for(const auto& ms : set.MultiSamples().data.multiSampleEntries)
	{
		if(!this->multiSampleIds.Contains(ms.id))
			continue;

		for(uint8 index : ms.keyZoneIndex)
		{
			if(index == Unsigned<uint8>::Max())
				continue;

			uint8 sampleNumber = set.MultiSamples().data.keyboardZones[index].sampleNumber;
			if(sampleNumber != Unsigned<uint8>::Max())
				this->sampleIds.Insert(set.MultiSamples().data.sampleEntries[sampleNumber].id);
		}
	}
}

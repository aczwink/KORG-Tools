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
//Local
#include <libkorg/MultiSamples/MultiSamplesData.hpp>

namespace libKORG
{
	class MultiSamplesIndex
	{
	public:
		//Constructor
		MultiSamplesIndex(const MultiSamples::MultiSamplesData& data);

		//Inline
		inline const MultiSamples::MultiSampleEntry& GetMultiSampleEntryById(uint64 id)
		{
			return this->data.multiSampleEntries[this->multiSampleIdMap.Get(id)];
		}

		inline const MultiSamples::SampleEntry& GetSampleEntryById(uint64 id)
		{
			return this->data.sampleEntries[this->sampleIdMap.Get(id)];
		}

	private:
		//Members
		const MultiSamples::MultiSamplesData& data;
		StdXX::BinaryTreeMap<uint64, uint32> multiSampleIdMap;
		StdXX::BinaryTreeMap<uint64, uint32> sampleIdMap;
	};
}
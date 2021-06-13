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
#include <libkorg/Set.hpp>

namespace libKORG
{
	class IdsCorrector
	{
	public:
		//Constructor
		inline IdsCorrector(Set& set) : set(set)
		{
			this->errorCounter = 0;
		}

		//Properties
		inline uint32 ErrorCount() const
		{
			return this->errorCounter;
		}

		//Methods
		void Correct();

	private:
		//Members
		Set& set;
		StdXX::BinaryTreeMap<uint64, uint32> sampleIdToIndexMap;
		StdXX::BinaryTreeMap<uint64, uint32> multiSampleIdToIndexMap;
		uint32 errorCounter;

		//Methods
		void CorrectMultiSampleAssignment(Sound::OSCMultiSampleSettings& multiSampleSettings);
		void CorrectSamples();
		void RemoveSampleFromIndex(uint32 index);
	};
}
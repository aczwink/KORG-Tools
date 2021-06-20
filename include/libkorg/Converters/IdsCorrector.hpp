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
		struct ErrorCounts
		{
			uint32 missingDrumSamplesCount = 0;
			uint32 missingSamplesCount = 0;
		};
	public:
		//Constructor
		inline IdsCorrector(Set& set) : set(set)
		{
		}

		//Properties
		inline const ErrorCounts& Errors() const
		{
			return this->errorCounts;
		}

		inline bool ErrorsDetected() const
		{
			return (this->errorCounts.missingDrumSamplesCount > 0) or (this->errorCounts.missingSamplesCount > 0);
		}

		//Methods
		void Correct();

	private:
		//Members
		Set& set;
		StdXX::BinaryTreeMap<uint64, uint32> drumSampleIdToIndexMap;
		StdXX::BinaryTreeMap<uint64, uint32> multiSampleIdToIndexMap;
		ErrorCounts errorCounts;

		//Methods
		void BuildDrumSampleIndex();
		void BuildMultiSampleIndex();
		StdXX::BinaryTreeMap<uint64, uint32> BuildSampleIndex();
		void CorrectDrumSampleAssignment(Sound::LayerEntry& layerEntry);
		void CorrectDrumSampleAssignments();
		void CorrectMultiSampleAssignment(Sound::OSCMultiSampleSettings& multiSampleSettings);
		void CorrectMultiSampleAssignments();
		void CorrectSamples();
		void RemoveSampleFromIndex(uint32 index);
		void RenumberDrumSampleAssignments();
	};
}
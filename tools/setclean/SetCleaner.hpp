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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX;

class SetCleaner
{
public:
	//Constructor
	inline SetCleaner(Set& set) : set(set), setIndex(set)
	{
	}

	//Methods
	void RemoveUnusedDrumSamples();
	void RemoveUnusedMultiSamples();
	void RemoveUnusedSamples(bool showAll);
	void RemoveUnusedSounds(bool ignorePerformanceAccSettings, bool ignoreSTS, bool showAll);

private:
	//Members
	Set& set;
	const SetIndex setIndex;
	BinaryTreeSet<uint64> markedDrumSamples;
	BinaryTreeSet<uint64> markedMultiSamples;
	BinaryTreeMap<uint64, uint32> markedSamples;
	BinaryTreeMap<ProgramChangeSequence, uint32> markedSounds;

	//Methods
	void ProcessDrumSamples();
	void ProcessMultiSamples();
	void ProcessPads();
	void ProcessPerformances(bool ignorePerformanceAccSettings);
	void ProcessSounds();
	void ProcessSTS(bool ignoreSTS, const SingleTouchSettings& sts);
	void ProcessStyle(const Style::StyleData& styleData);
	void ProcessStyles(bool ignoreSTS);
	void RemoveUnreferencedMultiSample(uint32 index);
	void RemoveUnreferencedSample(uint32 index);

	//Inline
	inline void IncrementSampleReferenceCount(uint64 sampleId)
	{
		this->markedSamples[sampleId]++;
	}

	inline void IncrementSoundReferenceCount(const ProgramChangeSequence& soundProgramChangeSeq)
	{
		this->markedSounds[soundProgramChangeSeq]++;
	}

	template<typename STSData>
	inline void ProcessSTSData(const STSData& data, bool ignoreSTS)
	{
		if(!ignoreSTS)
		{
			for (const auto &kbdSettings : data.keyboardSettings)
			{
				for (const auto &track : kbdSettings.trackSettings)
					this->IncrementSoundReferenceCount(track.soundProgramChangeSeq);
			}
		}

		for(const auto& track : data.accompanimentSettings.trackSettings)
			this->IncrementSoundReferenceCount(track.soundProgramChangeSeq);
	}
};
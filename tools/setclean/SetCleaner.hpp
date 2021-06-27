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
	void RemoveUnusedMultiSamples();
	void RemoveUnusedSamples();
	void RemoveUnusedSounds(bool ignoreSTS);

private:
	//Members
	Set& set;
	const SetIndex setIndex;
	BinaryTreeSet<uint64> markedDrumSamples;
	BinaryTreeSet<uint64> markedMultiSamples;
	BinaryTreeSet<uint64> markedSamples;
	BinaryTreeSet<ProgramChangeSequence> markedSounds;

	//Methods
	void ProcessDrumSamples();
	void ProcessMultiSamples();
	void ProcessPads();
	void ProcessPerformances();
	void ProcessSounds();
	void ProcessSTS(bool ignoreSTS, const SingleTouchSettings& sts);
	void ProcessStyle(const Style::StyleData& styleData);
	void ProcessStyles(bool ignoreSTS);
	void RemoveUnreferencedMultiSample(uint32 index);
	void RemoveUnreferencedSample(uint32 index);
};
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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX;

struct ResourceSelection
{
	BinaryTreeSet<uint64> multiSamples;
	BinaryTreeSet<const PerformanceObject*> performances;
	BinaryTreeSet<uint64> samples;
	bool requireFreeSoundSlot = false;
	BinaryTreeSet<ProgramChangeSequence> sounds;
};

class ResourceSelector
{
public:
	//Constructor
	inline ResourceSelector(const Set& sourceSet, const Set& targetSet, const SetIndex& setIndex, const MultiSamplesIndex& multiSamplesIndex)
		: sourceSet(sourceSet), targetSet(targetSet), setIndex(setIndex), multiSamplesIndex(multiSamplesIndex)
	{
		this->includeErrorneousData = true;
	}

	//Properties
	inline const ResourceSelection& Selection() const
	{
		return this->selection;
	}

	//Methods
	void PrintResults();
	void Select();

private:
	//Members
	const Set& sourceSet;
	const Set& targetSet;
	const SetIndex& setIndex;
	const MultiSamplesIndex& multiSamplesIndex;
	ResourceSelection selection;
	bool includeErrorneousData;

	//Methods
	uint32 ComputeTargetSampleSize(uint64 sampleId) const;
	uint32 ComputeUsedSampleRAMSize() const;
	bool SelectMultiSample(const MultiSamples::MultiSampleEntry& multiSampleEntry);
	bool SelectPCMSample(const MultiSamples::SampleEntry& sampleEntry);
	bool SelectPerformance(const PerformanceObject& performanceObject);
	void SelectPerformances();
	bool SelectSound(const ProgramChangeSequence& programChangeSequence);
	void SelectSounds();

	//Inline
	inline bool HasAvailablePerformanceSlot() const
	{
		auto bankIds = this->targetSet.model.GetBankSetup().performanceBanks.factoryBankIds;
		auto nBanks = bankIds.end - bankIds.start + 1; //end is inclusive
		auto nAvailableSlots = nBanks * 5 * 8; //5 pages and 8 performances per page
		uint32 nUsedSlots = this->selection.performances.GetNumberOfElements();
		return nUsedSlots < nAvailableSlots;
	}

	inline bool HasAvailableSoundSlot() const
	{
		uint32 nMaxSounds = this->targetSet.model.GetBankSetup().soundBanks.nUserBanks * 16 * 8; //16 pages and 8 sounds per page
		uint32 nUsedSounds = this->selection.sounds.GetNumberOfElements() + this->selection.requireFreeSoundSlot;
		return nUsedSounds < nMaxSounds;
	}

	inline bool SelectMultiSample(uint64 id)
	{
		auto entry = this->multiSamplesIndex.GetMultiSampleEntryById(id);
		if(entry)
			return this->SelectMultiSample(*entry);
		return this->includeErrorneousData;
	}

	inline bool SelectPCMSample(uint64 id)
	{
		auto entry = this->multiSamplesIndex.GetSampleEntryById(id);
		if(entry)
			return this->SelectPCMSample(*entry);
		return this->includeErrorneousData;
	}
};
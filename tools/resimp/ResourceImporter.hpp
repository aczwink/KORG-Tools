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

struct Config
{
	Optional<BankSlot<PerformanceBankNumber>> performanceInsertSlot;
	Optional<BankSlot<SoundBankNumber>> soundInsertSlot;
};

class ResourceImporter
{
public:
	//Constructor
	inline ResourceImporter(const Set& sourceSet, Set& targetSet, const Config& config) : sourceSet(sourceSet), targetSet(targetSet),
																	sourceSetIndex(sourceSet), sourceMultiSamplesIndex(sourceSet.MultiSamples().data),
																	targetMultiSamplesIndex(targetSet.MultiSamples().data), config(config)
	{
	}

	//Methods
	void ImportPerformance(const PerformanceBankNumber& bankNumber, uint8 pos);

private:
	//Members
	const Set& sourceSet;
	Set& targetSet;
	Config config;
	SetIndex sourceSetIndex;
	MultiSamplesIndex sourceMultiSamplesIndex;
	MultiSamplesIndex targetMultiSamplesIndex;
	BinaryTreeMap<uint64, uint32> importedMultiSampleIds;
	BinaryTreeMap<uint64, uint32> importedSampleIds;
	BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence> importedSounds;

	//Methods
	bool CheckIfSoundExists(const Sound::SoundData& soundData);
	bool ImportMultiSample(uint64 id);
	bool ImportSample(uint64 id);
	bool ImportSound(const ProgramChangeSequence& programChangeSequence);
	bool ImportSoundDependencies(const Sound::SoundData& soundData);
	void SaveChanges();

	//Inline
	inline uint16 MapMultiSampleIdToIndex(uint64 id)
	{
		if(this->importedMultiSampleIds.Contains(id))
			return this->importedMultiSampleIds.Get(id);
		return this->targetMultiSamplesIndex.GetMultiSampleEntryIndex(id);
	}

	template<typename PerfType>
	void ImportPerformanceData(const PerfType& source, const BankSlot<PerformanceBankNumber>& slot, const String& name)
	{
		for(const auto& trackSettings : source.keyboardSettings.trackSettings)
		{
			if(!this->ImportSound(trackSettings.soundProgramChangeSeq))
				return;
		}
		UniquePointer<PerfType> newPerformance = new PerfType(source);
		for(auto& trackSettings : newPerformance->keyboardSettings.trackSettings)
		{
			if(this->importedSounds.Contains(trackSettings.soundProgramChangeSeq))
				trackSettings.soundProgramChangeSeq = this->importedSounds.Get(trackSettings.soundProgramChangeSeq);
		}

		this->targetSet.performanceBanks[slot.bankNumber].SetObject(name, slot.pos, new PerformanceObject(Move(newPerformance)));
	}
};
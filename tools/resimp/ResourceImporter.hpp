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
	uint8 drumKitStartPos = 0;
	Optional<BankSlot<PerformanceBankNumber>> performanceInsertSlot;
	Optional<BankSlot<SoundBankNumber>> soundInsertSlot;
	Optional<BankSlot<StyleBankNumber>> styleInsertSlot;
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
	void ImportStyle(const StyleBankNumber& bankNumber, uint8 pos, bool ignoreSTSSounds);

private:
	//Members
	const Set& sourceSet;
	Set& targetSet;
	Config config;
	SetIndex sourceSetIndex;
	MultiSamplesIndex sourceMultiSamplesIndex;
	MultiSamplesIndex targetMultiSamplesIndex;
	BinaryTreeMap<uint64, uint32> importedDrumSampleIds;
	BinaryTreeMap<uint64, uint32> importedMultiSampleIds;
	BinaryTreeMap<uint64, uint32> importedSampleIds;
	BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence> importedSounds;

	//Methods
	Optional<BankSlot<SoundBankNumber>> FindSoundLocation(const Sound::SoundData& soundData);
	bool ImportDrumSample(uint64 id);
	bool ImportMultiSample(uint64 id);
	bool ImportSample(uint64 id);
	bool ImportSound(const ProgramChangeSequence& programChangeSequence);
	bool ImportSoundDependencies(const Sound::SoundData& soundData);
	UniquePointer<SingleTouchSettings> ImportSTS(const SingleTouchSettings& sts, bool ignoreSTSSounds);
	void SaveChanges();

	//Inline
	inline Optional<BankSlot<SoundBankNumber>> FindFreeSoundSlot(const Optional<BankSlot<SoundBankNumber>>& sourceLocation)
	{
		if(sourceLocation->bankNumber.IsDrumKit())
		{
			auto dkSoundBank = SoundBankNumber(11);
			auto freePos = this->targetSet.soundBanks[dkSoundBank].FindFreeSlot(this->config.drumKitStartPos);
			if(freePos.HasValue())
				return {{dkSoundBank, *freePos}};
			return {};
		}
		return this->targetSet.soundBanks.FindFreeSlot(this->config.soundInsertSlot.HasValue() ? this->config.soundInsertSlot.Value() : *sourceLocation);
	}

	inline uint16 MapDrumSampleIdToIndex(uint64 id)
	{
		if(this->importedDrumSampleIds.Contains(id))
			return this->importedDrumSampleIds.Get(id);
		return this->targetMultiSamplesIndex.GetDrumSampleEntryIndex(id);
	}

	inline uint16 MapMultiSampleIdToIndex(uint64 id)
	{
		if(this->importedMultiSampleIds.Contains(id))
			return this->importedMultiSampleIds.Get(id);
		return this->targetMultiSamplesIndex.GetMultiSampleEntryIndex(id);
	}

	inline uint16 MapSampleIdToIndex(uint64 id)
	{
		if(this->importedSampleIds.Contains(id))
			return this->importedSampleIds.Get(id);
		return this->targetMultiSamplesIndex.GetSampleEntryIndex(id);
	}

	inline ProgramChangeSequence MapSoundProgramChangeSequence(ProgramChangeSequence input) const
	{
		if(this->importedSounds.Contains(input))
			return this->importedSounds.Get(input);
		return input;
	}

	template<typename PerfType>
	bool ImportPerformanceData(const PerfType& source, const BankSlot<PerformanceBankNumber>& slot, const String& name)
	{
		for(const auto& trackSettings : source.keyboardSettings.trackSettings)
		{
			if(!this->ImportSound(trackSettings.soundProgramChangeSeq))
				return false;
		}
		UniquePointer<PerfType> newPerformance = new PerfType(source);
		for(auto& trackSettings : newPerformance->keyboardSettings.trackSettings)
		{
			trackSettings.soundProgramChangeSeq = this->MapSoundProgramChangeSequence(trackSettings.soundProgramChangeSeq);
		}

		this->targetSet.performanceBanks[slot.bankNumber].SetObject(name, slot.pos, new PerformanceObject(Move(newPerformance)));
		return true;
	}

	template<typename STSType>
	UniquePointer<SingleTouchSettings> ImportSTSData(const STSType& source, bool ignoreSTSSounds)
	{
		for(const auto& trackSettings : source.accompanimentSettings.trackSettings)
		{
			if(!this->ImportSound(trackSettings.soundProgramChangeSeq))
				return nullptr;
		}

		if(!ignoreSTSSounds)
		{
			for (const auto &keyboardSettings : source.keyboardSettings)
			{
				for (const auto &trackSettings : keyboardSettings.trackSettings)
				{
					if (!this->ImportSound(trackSettings.soundProgramChangeSeq))
						return nullptr;
				}
			}
		}

		UniquePointer<STSType> newSTS = new STSType(source);
		for(auto& keyboardSettings : newSTS->keyboardSettings)
		{
			for(auto& trackSettings : keyboardSettings.trackSettings)
			{
				trackSettings.soundProgramChangeSeq = this->MapSoundProgramChangeSequence(trackSettings.soundProgramChangeSeq);
			}
		}

		for(auto& trackSettings : newSTS->accompanimentSettings.trackSettings)
		{
			trackSettings.soundProgramChangeSeq = this->MapSoundProgramChangeSequence(trackSettings.soundProgramChangeSeq);
		}

		return new SingleTouchSettings(Move(newSTS));
	}
};
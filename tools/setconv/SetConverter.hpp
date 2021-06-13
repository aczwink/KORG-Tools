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
//Namespaces
using namespace libKORG;
using namespace StdXX;

class SetConverter
{
public:
	//Constructor
	inline SetConverter(const Set& sourceSet, const FileSystem::Path& targetSetPath, const Model& targetModel)
		: sourceSet(sourceSet),
		  multiSamplesIndex(sourceSet.MultiSamples().data),
		  setIndex(sourceSet),
		  targetSet(Set::Create(targetSetPath, targetModel))
	{
	}

	//Methods
	void Convert();

private:
	//Members
	const Set& sourceSet;
	MultiSamplesIndex multiSamplesIndex;
	SetIndex setIndex;
	Set targetSet;

	struct {
		BinaryTreeMap<uint64, uint32> integratedMultiSampleIds;
		BinaryTreeMap<uint64, uint32> integratedSampleIds;
		BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence> integratedSounds;
	} mapped;

	//Methods
	Sample::SampleData ConvertSampleIfRequired(const Sample::SampleData& sampleData);
	Multimedia::Packet* EncodeAudio(const Multimedia::Frame& frame, Multimedia::CodingFormatId codingFormatId, uint32 sampleRate) const;
	bool IntegrateMultiSample(const MultiSamples::MultiSampleEntry& multiSampleEntry);
	void IntegrateMultiSamples(const BinaryTreeSet<uint64>& selectedMultiSampleIds);
	void IntegratePerformances(const BinaryTreeMap<const PerformanceObject*, Tuple<PerformanceBankNumber, uint8, String>>& performanceAllocation, const ProgramChangeSequence& freeSoundSpotProgramChangeSequence);
	void IntegratePCM(const BinaryTreeSet<uint64>& selectedSampleIds);
	bool IntegratePCMSample(const MultiSamples::SampleEntry& sampleEntry);
	void IntegrateSounds(const BinaryTreeMap<ProgramChangeSequence, Tuple<SoundBankNumber, uint8, String>>& soundAllocation);

	//Inline
	inline uint32 MapMultiSampleIdToIndex(uint64 id)
	{
		if(this->mapped.integratedMultiSampleIds.Contains(id))
			return this->mapped.integratedMultiSampleIds.Get(id);

		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return id;
	}

	inline void ShowMultiSampleErrorMessage(uint64 multiSampleId)
	{
		auto entry = this->multiSamplesIndex.GetMultiSampleEntryById(multiSampleId);
		String id = String::HexNumber(multiSampleId, 16);
		String name = (id + u8" (" + entry.name + u8")");
		stdOut << u8"Can't integrate multisample with id '" << name << u8"' because of missing PCM samples." << endl;
	}
};
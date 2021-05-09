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
//Local
#include "SoundSelection.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

class SetConverter
{
public:
	//Constructor
	inline SetConverter(const FileSystem::Path& sourceSetPath, const FileSystem::Path& targetSetPath, const Model& targetModel)
		: sourceSet(sourceSetPath), targetSet(Set::Create(targetSetPath)), targetModel(targetModel)
	{
	}

	//Methods
	void Convert();

private:
	//Members
	const Set sourceSet;
	Set targetSet;
	const Model& targetModel;

	//Methods
	uint32 ComputeRequiredSampleRAMSize(const Set& set, const SoundSelection& selection);
	uint32 ComputeTargetSampleSize(const Sample::SampleData& sampleData);
	Sample::SampleData ConvertSampleIfRequired(const Sample::SampleData& sampleData);
	Multimedia::Packet* EncodeAudio(const Multimedia::Frame& frame, Multimedia::CodingFormatId codingFormatId, uint32 sampleRate) const;
	void IntegratePCM(const Set& sourceSet, const SoundSelection& selectedSounds);
	DynamicArray<const SoundObject*> SelectSounds(const Set& set);
};
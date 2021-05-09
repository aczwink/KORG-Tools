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
using namespace libKORG::MultiSamples;
#include "FormattedOutputter.hpp"

class MultiSamplesOutputter
{
public:
	//Constructor
	inline MultiSamplesOutputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const MultiSamplesObject& multiSamplesObject) const;

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(uint32 index, const DrumSampleEntry& sampleEntry) const;
	void Output(uint32 index, const MultiSampleEntry& multiSampleEntry) const;
	void Output(uint32 index, const SampleEntry& sampleEntry) const;
	void Output(uint32 index, const KeyboardZone& keyboardZone) const;
};
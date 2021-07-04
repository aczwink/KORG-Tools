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
#include "../Format1.0/MultiSamplesFormat1_0Reader.hpp"

class MultiSamplesFormat3_0Reader : public MultiSamplesFormat1_0Reader
{
protected:
	//Methods
	void ReadDrumSampleUnknowns3And4(libKORG::MultiSamples::DrumSampleEntry &drumSampleEntry, StdXX::DataReader &dataReader) override;
	void ReadDrumSampleUnknowns9To12(libKORG::MultiSamples::DrumSampleEntry &drumSampleEntry, StdXX::DataReader &dataReader) override;
	int16 ReadSampleUnknown2(StdXX::DataReader &dataReader) override;
	void ReadSampleUnknown14(libKORG::MultiSamples::SampleEntry& sampleEntry, StdXX::DataReader &dataReader) override;
};
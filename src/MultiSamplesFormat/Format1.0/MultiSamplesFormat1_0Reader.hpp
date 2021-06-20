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
#include <StdXX.hpp>
#include <libkorg/MultiSamples/MultiSamplesData.hpp>

class MultiSamplesFormat1_0Reader
{
public:
	//Members
	libKORG::MultiSamples::MultiSamplesData data;

	//Methods
	void Read(StdXX::DataReader& dataReader);

protected:
	//Overridable
	virtual void ReadDrumSampleUnknowns3And4(libKORG::MultiSamples::DrumSampleEntry& drumSampleEntry, StdXX::DataReader& dataReader);
	virtual void ReadDrumSampleUnknowns9To12(libKORG::MultiSamples::DrumSampleEntry& drumSampleEntry, StdXX::DataReader& dataReader);
	virtual int16 ReadSampleUnknown2(StdXX::DataReader& dataReader);
	virtual void ReadSampleUnknown14(libKORG::MultiSamples::SampleEntry& sampleEntry, StdXX::DataReader& dataReader);

private:
	//Methods
	void ReadDrumSamples(uint16 nDrumSampleEntries, StdXX::DataReader& dataReader);
	void ReadExtendedSampleInfo(libKORG::MultiSamples::SampleEntry& sampleEntry, StdXX::DataReader& dataReader);
	void ReadKeyboardZones(uint16 nEntries, StdXX::DataReader& dataReader);
	void ReadMultiSamples(uint16 nMultiSampleEntries, StdXX::DataReader& dataReader);
	void ReadSamples(uint16 nSampleEntries, StdXX::DataReader& dataReader);
};
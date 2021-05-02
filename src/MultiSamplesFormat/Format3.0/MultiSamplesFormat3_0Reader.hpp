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
#include <StdXX.hpp>
#include <libkorg/MultiSamples/MultiSamplesData.hpp>

class MultiSamplesFormat3_0Reader
{
public:
	//Members
	libKORG::MultiSamples::MultiSamplesData data;

	//Methods
	void Read(StdXX::DataReader& dataReader);

private:
	//Methods
	void ReadDrumSamples(uint16 nDrumSampleEntries, StdXX::DataReader& dataReader);
	void ReadMultiSamples(uint16 nMultiSampleEntries, StdXX::DataReader& dataReader);
	void ReadSamples(uint16 nSampleEntries, StdXX::DataReader& dataReader);
};
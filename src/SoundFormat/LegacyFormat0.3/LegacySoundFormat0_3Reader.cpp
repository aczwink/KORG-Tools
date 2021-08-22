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
//Class header
#include "LegacySoundFormat0_3Reader.hpp"
//Namespaces
using namespace libKORG::Sound;
using namespace StdXX;

//Protected methods
uint8 LegacySoundFormat0_3Reader::ReadLowPriority(DataReader &dataReader)
{
	return dataReader.ReadByte();
}

void LegacySoundFormat0_3Reader::ReadUnknowns21(OscillatorData &oscillatorData, DataReader &dataReader)
{
	dataReader.ReadBytes(oscillatorData.unknown34, sizeof(oscillatorData.unknown34));
	this->SkipUnknownBytes(dataReader);
}

void LegacySoundFormat0_3Reader::SkipUnknownBytes(DataReader &dataReader)
{
	dataReader.Skip(4);
}
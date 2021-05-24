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
#include "SoundFormat0_0Reader.hpp"
//Namespaces
using namespace libKORG::Sound;
using namespace StdXX;

//Protected methods
void SoundFormat0_0Reader::ReadFX(libKORG::Sound::EffectData& effectData, DataReader &dataReader)
{
	effectData.unknown1 = dataReader.ReadByte();
	dataReader.ReadBytes(effectData.unknown2, sizeof(effectData.unknown2));
	effectData.unknown3 = dataReader.ReadByte();
	effectData.unknown4 = 1;
}

uint16 SoundFormat0_0Reader::ReadMultiSampleMap(DataReader &dataReader)
{
	return dataReader.ReadUInt16();
}

void SoundFormat0_0Reader::ReadUnknowns10(DataReader &dataReader)
{
	dataReader.ReadBytes(this->data.unknown4, sizeof(this->data.unknown4));
}

void SoundFormat0_0Reader::ReadUnknowns11(OscillatorData& oscillatorData, DataReader &dataReader)
{
	oscillatorData.unknown2 = dataReader.ReadByte();
}

void SoundFormat0_0Reader::SkipUnknownBytes(DataReader &dataReader)
{
}
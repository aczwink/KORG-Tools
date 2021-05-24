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
#include "SoundFormat1_0Reader.hpp"
//Namespaces
using namespace libKORG::Sound;
using namespace StdXX;

//Protected methods
void SoundFormat1_0Reader::ReadMaxTimeAndRange(DataReader &dataReader)
{
	this->data.maxTime = dataReader.ReadUInt16();
	this->data.maxRange = dataReader.ReadByte();
}

void SoundFormat1_0Reader::ReadOSCTrigger(OscillatorData &oscillatorData, DataReader &dataReader)
{
	oscillatorData.oscTriggerMode = static_cast<OSCTriggerMode>(dataReader.ReadByte());
	oscillatorData.scException = dataReader.ReadByte();
}
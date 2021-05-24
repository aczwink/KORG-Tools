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
#include "SoundFormat2_0Reader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Sound;
using namespace StdXX;

//Protected methods
void SoundFormat2_0Reader::ReadEffects(DataReader &dataReader)
{
	for(uint8 i = 0; i < 4; i++)
		this->ReadFX(this->data.effects[i], dataReader);
}

void SoundFormat2_0Reader::ReadFX(libKORG::Sound::EffectData& effectData, DataReader &dataReader)
{
	SoundFormat1_0Reader::ReadFX(effectData, dataReader);
	effectData.unknown4 = dataReader.ReadByte();
}

void SoundFormat2_0Reader::ReadOscillatorEqualizer(OscillatorData &oscillatorData, DataReader &dataReader)
{
	this->ReadEqualizerData(oscillatorData.equalizer, dataReader);
}

void SoundFormat2_0Reader::ReadOscillatorMultiSamplesData(OSCMultiSampleSettings &oscMultiSampleSettings, DataReader &dataReader)
{
	SoundFormat1_0Reader::ReadOscillatorMultiSamplesData(oscMultiSampleSettings, dataReader);

	oscMultiSampleSettings.reversed = this->ReadBool(dataReader);
	oscMultiSampleSettings.offset = static_cast<Offset>(dataReader.ReadByte());
}

void SoundFormat2_0Reader::ReadUnknowns1(DataReader &dataReader)
{
	this->data.unknown22 = dataReader.ReadByte();
	this->data.unknown23 = dataReader.ReadByte();
	this->data.unknown3 = dataReader.ReadByte();
}

void SoundFormat2_0Reader::ReadUnknowns2(OscillatorData &oscillatorData, DataReader &dataReader)
{
	oscillatorData.unknown31 = dataReader.ReadByte();
	oscillatorData.unknown32 = dataReader.ReadByte();
}

void SoundFormat2_0Reader::ReadUnknowns4(KeyTableEntry &keyTableEntry, DataReader &dataReader)
{
	keyTableEntry.unknown12 = dataReader.ReadByte();
}

void SoundFormat2_0Reader::ReadLayerEntryDetails(LayerEntry &layerEntry, DataReader &dataReader)
{
	int8 b = dataReader.ReadByte();
	layerEntry.unknown13 = (b * 330 + 32768);

	layerEntry.unknown14 = dataReader.ReadByte();

	this->ReadEqualizerData(layerEntry.drumSampleEqualizer, dataReader);

	dataReader.ReadBytes(layerEntry.unknown21, sizeof(layerEntry.unknown21));

	layerEntry.intensity = dataReader.ReadByte();
}

void SoundFormat2_0Reader::SkipUnknownKeyTableEntryByte(DataReader &dataReader)
{
}

//Private methods
void SoundFormat2_0Reader::ReadEqualizerData(EqualizerData &equalizerData, DataReader &dataReader)
{
	equalizerData.enable = this->ReadBool(dataReader);
	equalizerData.trim = dataReader.ReadByte();
	equalizerData.lowGainTimes2 = dataReader.ReadByte();
	equalizerData.freq = dataReader.ReadByte();
	equalizerData.midGainTimes2 = dataReader.ReadByte();
	equalizerData.highGainTimes2 = dataReader.ReadByte();
}
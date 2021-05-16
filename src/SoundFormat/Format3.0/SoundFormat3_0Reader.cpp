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
#include "SoundFormat3_0Reader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Sound;
using namespace StdXX;

//Public methods
void SoundFormat3_0Reader::Read(DataReader& dataReader)
{
	this->data.multiSampleMap = dataReader.ReadUInt16();

	this->ReadFlags(dataReader);

	for(uint8 i = 0; i < 4; i++)
		this->ReadFX(this->data.effects[i], dataReader);

	dataReader.ReadBytes(this->data.unknown2, sizeof(this->data.unknown2));
	this->data.unknown3 = dataReader.ReadByte();

	this->data.unknownPacked = dataReader.ReadByte();
	this->data.lowPriority = dataReader.ReadByte();

	this->ReadUnknownData(dataReader);

	this->data.maxTime = dataReader.ReadUInt16();
	this->data.maxRange = dataReader.ReadByte();

	uint8 nOscillators = this->data.unknownPacked & 0x1F; //at least 0x40 can be set in that bit field. No idea what it is
	this->data.oscillators.Resize(nOscillators);
	for(uint8 i = 0; i < nOscillators; i++)
		this->ReadOscillatorData(this->data.oscillators[i], dataReader);

	if(this->data.voiceAssignModeFlags.IsSet(libKORG::Sound::VoiceAssignModeFlags::IsDrumKit))
	{
		DrumKitSoundData dkData;
		this->ReadDrumKitSoundData(dkData, dataReader);
		this->data.drumKitData = dkData;
	}
}

//Private methods
void SoundFormat3_0Reader::ReadDrumKitSoundData(DrumKitSoundData& drumKitSoundData, DataReader& dataReader)
{
	for(KeyTableEntry& entry : drumKitSoundData.keyTable)
	{
		this->ReadKeyTableEntry(entry, dataReader);
	}

	ASSERT_EQUALS(128, dataReader.ReadByte()); //number of key table entries
	uint8 nLayers = dataReader.ReadByte();

	for(uint8 i = 0; i < nLayers; i++)
	{
		LayerEntry layerEntry;
		this->ReadLayerEntry(layerEntry, dataReader);
		drumKitSoundData.layers.Push(layerEntry);
	}
}

void SoundFormat3_0Reader::ReadKeyTableEntry(KeyTableEntry& keyTableEntry, DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadByte()); //probably the high byte of the index
	keyTableEntry.index = dataReader.ReadByte();
	keyTableEntry.nLayers = dataReader.ReadByte();

	dataReader.ReadBytes(keyTableEntry.unknown1, sizeof(keyTableEntry.unknown1));
	dataReader.ReadBytes(keyTableEntry.velocitySampleSwitches, sizeof(keyTableEntry.velocitySampleSwitches));

	ASSERT_EQUALS(127, dataReader.ReadByte());
	ASSERT_EQUALS(127, dataReader.ReadByte());
	ASSERT_EQUALS(127, dataReader.ReadByte());
}

void SoundFormat3_0Reader::ReadLayerEntry(LayerEntry& layerEntry, DataReader &dataReader)
{
	layerEntry.unknown1 = dataReader.ReadByte();

	uint8 rev = dataReader.ReadByte();
	ASSERT((rev == 0) || (rev == 2), String::Number(rev));
	layerEntry.reversed = static_cast<Reversed>(rev);

	layerEntry.unknown11 = dataReader.ReadByte();

	layerEntry.multiSampleNumber = dataReader.ReadByte();
	layerEntry.level = dataReader.ReadByte();
	layerEntry.transpose = dataReader.ReadByte();
	layerEntry.tune = dataReader.ReadByte();
	layerEntry.attack = dataReader.ReadByte();
	layerEntry.decay = dataReader.ReadByte();
	layerEntry.cutoff = dataReader.ReadByte();
	layerEntry.resonance = dataReader.ReadByte();

	dataReader.ReadBytes(layerEntry.unknown12, sizeof(layerEntry.unknown12));

	layerEntry.drumSampleEqualizer.enable = this->ReadBool(dataReader);
	layerEntry.drumSampleEqualizer.trim = dataReader.ReadByte();
	layerEntry.drumSampleEqualizer.lowGainTimes2 = dataReader.ReadByte();
	layerEntry.drumSampleEqualizer.freq = dataReader.ReadByte();
	layerEntry.drumSampleEqualizer.midGainTimes2 = dataReader.ReadByte();
	layerEntry.drumSampleEqualizer.highGainTimes2 = dataReader.ReadByte();

	dataReader.ReadBytes(layerEntry.unknown21, sizeof(layerEntry.unknown21));

	layerEntry.intensity = dataReader.ReadByte();

	dataReader.ReadBytes(layerEntry.unknown22, sizeof(layerEntry.unknown22));
}
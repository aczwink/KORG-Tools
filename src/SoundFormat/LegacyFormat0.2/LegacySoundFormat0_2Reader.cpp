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
#include "LegacySoundFormat0_2Reader.hpp"
//Namespaces
using namespace libKORG::Sound;
using namespace StdXX;

//Protected methods
void LegacySoundFormat0_2Reader::ReadDrumKitSoundData(DrumKitSoundData& drumKitSoundData, DataReader& dataReader)
{
	for(KeyTableEntry& entry : drumKitSoundData.keyTable)
	{
		this->ReadKeyTableEntry(entry, dataReader);
	}

	Bitfield<uint16> packed = dataReader.ReadUInt16();
	bool hasId = packed.IsBitSet(15);
	uint16 nLayers = packed.Get(0, 15);

	for(uint16 i = 0; i < nLayers; i++)
	{
		LayerEntry layerEntry;
		this->ReadLayerEntry(layerEntry, hasId, dataReader);
		drumKitSoundData.layers.Push(layerEntry);
	}
}

void LegacySoundFormat0_2Reader::ReadKeyRange(KeyRange& keyRange, DataReader &dataReader)
{
	keyRange.bottomKey = dataReader.ReadByte();
	keyRange.topKey = dataReader.ReadByte();
}

void LegacySoundFormat0_2Reader::ReadLayerEntryDetails(LayerEntry &layerEntry, DataReader &dataReader)
{
	dataReader.Skip(1); //?
}

uint64 LegacySoundFormat0_2Reader::ReadMultiSampleId(DataReader &dataReader)
{
	return dataReader.ReadUInt64();
}

uint16 LegacySoundFormat0_2Reader::ReadMultiSampleMap(DataReader &dataReader)
{
	return 2;
}

void LegacySoundFormat0_2Reader::ReadScaledVelocity(OscillatorData &oscillatorData, DataReader &dataReader)
{
	oscillatorData.scaledVelocityBottom = dataReader.ReadByte();
	oscillatorData.scaledVelocityTop = dataReader.ReadByte();
}

void LegacySoundFormat0_2Reader::ReadUnknowns4(KeyTableEntry &keyTableEntry, DataReader &dataReader)
{
}

void LegacySoundFormat0_2Reader::SkipUnknownKeyTableEntryByte(DataReader &dataReader)
{
	dataReader.Skip(1); //?
}

//Private methods
void LegacySoundFormat0_2Reader::ReadKeyTableEntry(KeyTableEntry& keyTableEntry, DataReader &dataReader)
{
	keyTableEntry.index = dataReader.ReadUInt16();
	keyTableEntry.nLayers = dataReader.ReadByte();

	dataReader.ReadBytes(keyTableEntry.unknown11, sizeof(keyTableEntry.unknown11));
	this->ReadUnknowns4(keyTableEntry, dataReader);
	dataReader.ReadBytes(keyTableEntry.unknown13, sizeof(keyTableEntry.unknown13));

	dataReader.ReadBytes(keyTableEntry.velocitySampleSwitches, sizeof(keyTableEntry.velocitySampleSwitches));

	ASSERT_EQUALS(127, dataReader.ReadByte());
	ASSERT_EQUALS(127, dataReader.ReadByte());
	ASSERT_EQUALS(127, dataReader.ReadByte());

	this->SkipUnknownKeyTableEntryByte(dataReader);
}

void LegacySoundFormat0_2Reader::ReadLayerEntry(LayerEntry& layerEntry, bool hasId, DataReader &dataReader)
{
	layerEntry.sampleBankNumber = dataReader.ReadByte();

	uint8 rev = dataReader.ReadByte();
	ASSERT((rev == 0)
		   || (rev == 1)
		   || (rev == 2)
		   || (rev == 3), String::Number(rev));
	layerEntry.reversed = static_cast<Reversed>(rev);

	layerEntry.drumSampleNumber = dataReader.ReadUInt16();

	layerEntry.level = dataReader.ReadByte();
	layerEntry.transpose = dataReader.ReadByte();
	layerEntry.tune = dataReader.ReadByte();
	layerEntry.attack = dataReader.ReadByte();
	layerEntry.decay = dataReader.ReadByte();
	layerEntry.cutoff = dataReader.ReadByte();
	layerEntry.resonance = dataReader.ReadByte();

	this->ReadLayerEntryDetails(layerEntry, dataReader);

	if(hasId)
		layerEntry.drumSampleId = dataReader.ReadUInt64();
	else
		layerEntry.drumSampleId = 0;
}
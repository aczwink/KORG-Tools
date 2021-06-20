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
#include "LegacySoundFormat0_0Reader.hpp"
//Namespaces
using namespace libKORG::Sound;
using namespace StdXX;

//Public methods
void LegacySoundFormat0_0Reader::Read(DataReader &dataReader)
{
	this->data.multiSampleMap = this->ReadMultiSampleMap(dataReader);
	this->ReadName(dataReader);

	this->ReadUnknowns20(dataReader);

	this->ReadEffects(dataReader);

	dataReader.ReadBytes(this->data.unknown21, sizeof(this->data.unknown21));
	this->ReadUnknowns1(dataReader);
	this->data.unknownPacked = dataReader.ReadByte();

	this->data.lowPriority = this->ReadLowPriority(dataReader);

	this->ReadUnknowns10(dataReader);
	this->ReadUnknownData(dataReader);

	this->ReadMaxTimeAndRange(dataReader);

	uint8 nOscillators = this->data.unknownPacked & 0x3F; //at least 0x40 can be sourceSet in that bit field. No idea what it is
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

//Protected methods
void LegacySoundFormat0_0Reader::ReadDrumKitSoundData(DrumKitSoundData &drumKitSoundData, DataReader &dataReader)
{
	for(auto & keyTableEntry : drumKitSoundData.keyTable)
	{
		LayerEntry layerEntries[2];
		for(auto& layerEntry : layerEntries)
		{
			layerEntry.sampleBankNumber = dataReader.ReadByte();
			layerEntry.reversed = static_cast<Reversed>(dataReader.ReadByte());
			layerEntry.drumSampleNumber = dataReader.ReadUInt16();

			layerEntry.level = dataReader.ReadInt8();
			layerEntry.transpose = dataReader.ReadInt8();
			layerEntry.tune = dataReader.ReadInt8();
			layerEntry.attack = dataReader.ReadInt8();
			layerEntry.decay = dataReader.ReadInt8();
			layerEntry.cutoff = dataReader.ReadInt8();
			layerEntry.resonance = dataReader.ReadInt8();

			layerEntry.drumSampleId = 0;

			dataReader.Skip(1); //?
		}

		keyTableEntry.index = drumKitSoundData.layers.GetNumberOfElements();

		dataReader.ReadBytes(keyTableEntry.unknown11, sizeof(keyTableEntry.unknown11));
		dataReader.ReadBytes(keyTableEntry.unknown13, sizeof(keyTableEntry.unknown13));

		uint8 velocitySampleSwitch = dataReader.ReadByte();
		if(velocitySampleSwitch == 1)
		{
			//TODO: set velocity sample switch
			drumKitSoundData.layers.Push(layerEntries[0]);
			keyTableEntry.nLayers = 1;
		}
		else if(velocitySampleSwitch == 127)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
		else
		{
			//TODO: set velocity sample switch
			drumKitSoundData.layers.Push(layerEntries[1]);
			drumKitSoundData.layers.Push(layerEntries[0]);
			keyTableEntry.nLayers = 2;
		}

		dataReader.Skip(1); //?
	}
}

void LegacySoundFormat0_0Reader::ReadEffects(DataReader &dataReader)
{
	for(uint8 i = 0; i < 2; i++)
		this->ReadFX(this->data.effects[i], dataReader);
}

void LegacySoundFormat0_0Reader::ReadFX(libKORG::Sound::EffectData &effectData, DataReader &dataReader)
{
	effectData.unknown1 = 1;
	dataReader.ReadBytes(effectData.unknown2, 16);
	effectData.unknown3 = dataReader.ReadByte();
	effectData.unknown4 = 1;
}

void LegacySoundFormat0_0Reader::ReadKeyRange(KeyRange &keyRange, DataReader &dataReader)
{
}

uint8 LegacySoundFormat0_0Reader::ReadLowPriority(DataReader &dataReader)
{
	return 0;
}

void LegacySoundFormat0_0Reader::ReadMaxTimeAndRange(DataReader &dataReader)
{
}

uint64 LegacySoundFormat0_0Reader::ReadMultiSampleId(DataReader &dataReader)
{
	return 0;
}

uint16 LegacySoundFormat0_0Reader::ReadMultiSampleMap(DataReader &dataReader)
{
	return 1;
}

void LegacySoundFormat0_0Reader::ReadName(DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
	String name = textReader.ReadString(16);
}

void LegacySoundFormat0_0Reader::ReadOscillatorEqualizer(OscillatorData &oscillatorData, DataReader &dataReader)
{
}

void LegacySoundFormat0_0Reader::ReadOscillatorMultiSamplesData(OSCMultiSampleSettings& oscMultiSampleSettings, DataReader &dataReader)
{
	oscMultiSampleSettings.multiSampleId = this->ReadMultiSampleId(dataReader);

	Bitfield<uint16> packed = dataReader.ReadUInt16();

	oscMultiSampleSettings.source = static_cast<MultiSampleSource>(dataReader.ReadByte());
	oscMultiSampleSettings.level = dataReader.ReadByte();

	if(packed.IsBitSet(15))
		oscMultiSampleSettings.offset = Offset::NoAttack;
	if(packed.IsBitSet(14))
		oscMultiSampleSettings.reversed = true;
	oscMultiSampleSettings.multiSampleNumber = packed.Get(0, 12);
}

void LegacySoundFormat0_0Reader::ReadOSCTrigger(OscillatorData &oscillatorData, DataReader &dataReader)
{
}

void LegacySoundFormat0_0Reader::ReadScaledVelocity(OscillatorData &oscillatorData, DataReader &dataReader)
{
	oscillatorData.scaledVelocityBottom = oscillatorData.velocityZoneBottom;
	oscillatorData.scaledVelocityTop = oscillatorData.velocityZoneTop;
}

void LegacySoundFormat0_0Reader::ReadUnknowns1(DataReader &dataReader)
{
}

void LegacySoundFormat0_0Reader::ReadUnknowns2(OscillatorData &oscillatorData, DataReader &dataReader)
{
}

void LegacySoundFormat0_0Reader::ReadUnknowns10(DataReader &dataReader)
{
}

void LegacySoundFormat0_0Reader::ReadUnknowns11(OscillatorData &oscillatorData, DataReader &dataReader)
{
}

void LegacySoundFormat0_0Reader::ReadUnknowns21(OscillatorData &oscillatorData, DataReader &dataReader)
{
}

//Private methods
void LegacySoundFormat0_0Reader::ReadOscillatorData(OscillatorData &oscillatorData, DataReader &dataReader)
{
	auto& osc = oscillatorData;

	this->ReadOscillatorMultiSamplesData(osc.high, dataReader);
	this->ReadOscillatorMultiSamplesData(osc.low, dataReader);
	osc.oscTriggerModeDelay = static_cast<OSCTriggerModeDelay>(dataReader.ReadByte());
	osc.velocityMultiSampleSwitchLowHigh = dataReader.ReadByte();
	osc.velocityZoneBottom = dataReader.ReadByte();
	osc.velocityZoneTop = dataReader.ReadByte();

	this->ReadKeyRange(osc.keyboardRange, dataReader);

	this->ReadUnknowns11(oscillatorData, dataReader);

	this->ReadOSCTrigger(osc, dataReader);

	this->ReadUnknowns2(oscillatorData, dataReader);
	this->ReadUnknowns21(osc, dataReader);
	dataReader.ReadBytes(osc.unknown35, sizeof(osc.unknown35));

	osc.octaveTranspose = dataReader.ReadByte();
	osc.transpose = dataReader.ReadByte();
	osc.tune = dataReader.ReadInt16();

	dataReader.ReadBytes(osc.unknown4, sizeof(osc.unknown4));

	this->ReadScaledVelocity(osc, dataReader);

	dataReader.ReadBytes(osc.unknown51, sizeof(osc.unknown51));
	this->ReadOscillatorEqualizer(oscillatorData, dataReader);
	dataReader.ReadBytes(osc.unknown53, sizeof(osc.unknown53));
}

void LegacySoundFormat0_0Reader::ReadUnknownData(DataReader &dataReader)
{
	this->data.unknown5 = dataReader.ReadByte();
	this->data.unknown6 = dataReader.ReadByte();
	this->data.unknown7 = dataReader.ReadByte();
	this->data.unknown8 = dataReader.ReadByte();
	this->data.unknown9 = dataReader.ReadByte();
	this->data.unknown10 = dataReader.ReadByte();
	this->data.unknown11 = dataReader.ReadByte();
	this->data.unknown12 = dataReader.ReadByte();
	this->data.unknown13 = dataReader.ReadByte();
	this->data.unknown14 = dataReader.ReadByte();
	this->data.unknown15 = dataReader.ReadByte();
	this->data.unknown16 = dataReader.ReadByte();
	this->data.unknown17 = dataReader.ReadByte();
	this->data.unknown18 = dataReader.ReadByte();
}

void LegacySoundFormat0_0Reader::ReadUnknowns20(DataReader &dataReader)
{
	this->data.voiceAssignModeFlags = dataReader.ReadByte();

	dataReader.ReadBytes(this->data.unknown1, sizeof(this->data.unknown1));

	this->ReadKeyRange(this->data.transposeRange, dataReader);
}
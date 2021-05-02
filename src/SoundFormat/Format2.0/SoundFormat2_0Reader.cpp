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

//Public methods
void SoundFormat2_0Reader::Read(DataReader& dataReader)
{
	uint16 versionMaybe = dataReader.ReadUInt16();
	ASSERT((versionMaybe == 13) || (versionMaybe == 12), String::Number(versionMaybe));

	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
	String name = textReader.ReadString(16);

	//TODO: THIS METHOD IS (FROM HERE ON) UNTESTED!!!!! TESTDATA REQUIRED

	this->data.voiceAssignModeFlags = dataReader.ReadByte();

	dataReader.ReadBytes(this->data.unknown1, sizeof(this->data.unknown1));

	this->data.transposeRangeBottomKey = dataReader.ReadByte();
	this->data.transposeRangeTopKey = dataReader.ReadByte();

	dataReader.ReadBytes(this->data.unknown11, sizeof(this->data.unknown11));

	this->data.unknownPacked = dataReader.ReadByte();
	this->data.lowPriority = dataReader.ReadByte();

	dataReader.ReadBytes(this->data.unknown2, sizeof(this->data.unknown2));

	this->data.maxTime = dataReader.ReadUInt16();
	this->data.maxRange = dataReader.ReadByte();

	uint8 nOscillators = this->data.unknownPacked & 0x1F; //at least 0x40 can be set in that bit field. No idea what it is
	this->data.oscillators.Resize(nOscillators);
	for(uint8 i = 0; i < nOscillators; i++)
	{
		auto& osc = this->data.oscillators[i];

		this->ReadOscillatorData(osc.high, dataReader);
		this->ReadOscillatorData(osc.low, dataReader);
		osc.oscTriggerModeDelay = static_cast<OSCTriggerModeDelay>(dataReader.ReadByte());
		osc.velocityMultiSampleSwitchLowHigh = dataReader.ReadByte();
		osc.velocityZoneBottom = dataReader.ReadByte();
		osc.velocityZoneTop = dataReader.ReadByte();
		osc.keyboardRangeBottomKey = dataReader.ReadByte();
		osc.keyboardRangeTopKey = dataReader.ReadByte();

		osc.unknown2 = dataReader.ReadByte();

		osc.oscTriggerMode = static_cast<OSCTriggerMode>(dataReader.ReadByte());
		osc.oscOffWhenSoundControllersAreOn = this->ReadBool(dataReader);

		dataReader.ReadBytes(osc.unknown3, sizeof(osc.unknown3));

		osc.octaveTranspose = dataReader.ReadByte();
		osc.transpose = dataReader.ReadByte();
		osc.tune = dataReader.ReadInt16();

		dataReader.ReadBytes(osc.unknown4, sizeof(osc.unknown4));

		osc.scaledVelocityBottom = dataReader.ReadByte();
		osc.scaledVelocityTop = dataReader.ReadByte();

		dataReader.ReadBytes(osc.unknown5, sizeof(osc.unknown5));
	}

	if(this->data.voiceAssignModeFlags.IsSet(libKORG::Sound::VoiceAssignModeFlags::IsDrumKit))
	{
		DrumKitSoundData dkData;
		this->ReadDrumKitSoundData(dkData, dataReader);
		this->data.drumKitData = dkData;
	}
}

//Private methods
void SoundFormat2_0Reader::ReadDrumKitSoundData(DrumKitSoundData& drumKitSoundData, DataReader& dataReader)
{
	//TODO: THIS METHOD IS UNTESTED!!!!! TESTDATA REQUIRED

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

void SoundFormat2_0Reader::ReadKeyTableEntry(KeyTableEntry& keyTableEntry, DataReader &dataReader)
{
	//TODO: THIS METHOD IS UNTESTED!!!!! TESTDATA REQUIRED

	ASSERT_EQUALS(0, dataReader.ReadByte()); //probably the high byte of the index
	keyTableEntry.index = dataReader.ReadByte();
	keyTableEntry.nLayers = dataReader.ReadByte();

	dataReader.ReadBytes(keyTableEntry.unknown1, sizeof(keyTableEntry.unknown1));
	dataReader.ReadBytes(keyTableEntry.velocitySampleSwitches, sizeof(keyTableEntry.velocitySampleSwitches));

	ASSERT_EQUALS(127, dataReader.ReadByte());
	ASSERT_EQUALS(127, dataReader.ReadByte());
	ASSERT_EQUALS(127, dataReader.ReadByte());
}

void SoundFormat2_0Reader::ReadLayerEntry(LayerEntry& layerEntry, DataReader &dataReader)
{
	//TODO: THIS METHOD IS UNTESTED!!!!! TESTDATA REQUIRED

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

void SoundFormat2_0Reader::ReadOscillatorData(OSCMultiSampleSettings& oscMultiSampleSettings, DataReader &dataReader)
{
	//TODO: THIS METHOD IS UNTESTED!!!!! TESTDATA REQUIRED

	dataReader.ReadBytes(oscMultiSampleSettings.unknown1, sizeof(oscMultiSampleSettings.unknown1));
	oscMultiSampleSettings.multiSampleNumber = dataReader.ReadUInt16();
	oscMultiSampleSettings.unknown2 = dataReader.ReadByte();
	oscMultiSampleSettings.level = dataReader.ReadByte();
	oscMultiSampleSettings.reversed = this->ReadBool(dataReader);
	oscMultiSampleSettings.offset = static_cast<Offset>(dataReader.ReadByte());
}
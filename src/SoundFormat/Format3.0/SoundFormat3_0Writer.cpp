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
#include "SoundFormat3_0Writer.hpp"
//Namespaces
using namespace libKORG;

//Public methods
void SoundFormat3_0Writer::Write(const Sound::SoundData &soundData)
{
	this->dataWriter.WriteUInt16(soundData.multiSampleMap);

	this->dataWriter.WriteByte(soundData.voiceAssignModeFlags.encodedFlags);
	this->dataWriter.WriteBytes(soundData.unknown1, sizeof(soundData.unknown1));
	this->dataWriter.WriteByte(soundData.transposeRangeBottomKey.Encode());
	this->dataWriter.WriteByte(soundData.transposeRangeTopKey.Encode());

	for(uint8 i = 0; i < 4; i++)
	{
		auto& fx = soundData.effects[i];
		this->dataWriter.WriteByte(fx.unknown1);
		this->dataWriter.WriteBytes(fx.unknown2, sizeof(fx.unknown2));
		this->dataWriter.WriteByte(fx.unknown3);
		this->dataWriter.WriteByte(fx.unknown4);
	}

	this->dataWriter.WriteBytes(soundData.unknown2, sizeof(soundData.unknown2));
	this->dataWriter.WriteByte(soundData.unknown3);
	this->dataWriter.WriteByte(soundData.unknownPacked);
	this->dataWriter.WriteByte(soundData.lowPriority);
	this->dataWriter.WriteBytes(soundData.unknown4, sizeof(soundData.unknown4));
	this->dataWriter.WriteByte(soundData.unknown5);
	this->dataWriter.WriteByte(soundData.unknown6);
	this->dataWriter.WriteByte(soundData.unknown7);
	this->dataWriter.WriteByte(soundData.unknown8);
	this->dataWriter.WriteByte(soundData.unknown9);
	this->dataWriter.WriteByte(soundData.unknown10);
	this->dataWriter.WriteByte(soundData.unknown11);
	this->dataWriter.WriteByte(soundData.unknown12);
	this->dataWriter.WriteByte(soundData.unknown13);
	this->dataWriter.WriteByte(soundData.unknown14);
	this->dataWriter.WriteByte(soundData.unknown15);
	this->dataWriter.WriteByte(soundData.unknown16);
	this->dataWriter.WriteByte(soundData.unknown17);
	this->dataWriter.WriteByte(soundData.unknown18);
	this->dataWriter.WriteUInt16(soundData.maxTime);
	this->dataWriter.WriteByte(soundData.maxRange);

	for(const auto& osc : soundData.oscillators)
		this->WriteOscillatorData(osc);

	ASSERT_EQUALS(false, soundData.drumKitData.HasValue()); //TODO: implement writing drum kit data
}

//Private methods
void SoundFormat3_0Writer::WriteOscillatorData(const Sound::OscillatorData &oscillatorData)
{
	this->WriteOscillatorMultiSamplesData(oscillatorData.high);
	this->WriteOscillatorMultiSamplesData(oscillatorData.low);

	this->dataWriter.WriteByte(static_cast<byte>(oscillatorData.oscTriggerModeDelay));
	this->dataWriter.WriteByte(oscillatorData.velocityMultiSampleSwitchLowHigh);
	this->dataWriter.WriteByte(oscillatorData.velocityZoneBottom);
	this->dataWriter.WriteByte(oscillatorData.velocityZoneTop);
	this->dataWriter.WriteByte(oscillatorData.keyboardRangeBottomKey.Encode());
	this->dataWriter.WriteByte(oscillatorData.keyboardRangeTopKey.Encode());
	this->dataWriter.WriteByte(oscillatorData.unknown2);
	this->dataWriter.WriteByte(static_cast<byte>(oscillatorData.oscTriggerMode));
	this->dataWriter.WriteByte(oscillatorData.oscOffWhenSoundControllersAreOn);

	this->dataWriter.WriteBytes(oscillatorData.unknown3, sizeof(oscillatorData.unknown3));

	this->dataWriter.WriteByte(oscillatorData.octaveTranspose);
	this->dataWriter.WriteByte(oscillatorData.transpose);
	this->dataWriter.WriteInt16(oscillatorData.tune);

	this->dataWriter.WriteBytes(oscillatorData.unknown4, sizeof(oscillatorData.unknown4));

	this->dataWriter.WriteByte(oscillatorData.scaledVelocityBottom);
	this->dataWriter.WriteByte(oscillatorData.scaledVelocityTop);

	this->dataWriter.WriteBytes(oscillatorData.unknown5, sizeof(oscillatorData.unknown5));
}

void SoundFormat3_0Writer::WriteOscillatorMultiSamplesData(const Sound::OSCMultiSampleSettings &oscMultiSampleSettings)
{
	this->dataWriter.WriteUInt64(oscMultiSampleSettings.multiSampleId);
	this->dataWriter.WriteUInt16(oscMultiSampleSettings.multiSampleNumber);
	this->dataWriter.WriteByte(static_cast<byte>(oscMultiSampleSettings.source));
	this->dataWriter.WriteByte(oscMultiSampleSettings.level);
	this->dataWriter.WriteByte(oscMultiSampleSettings.reversed);
	this->dataWriter.WriteByte(static_cast<byte>(oscMultiSampleSettings.offset));
}

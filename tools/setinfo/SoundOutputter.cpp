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
#include "SoundOutputter.hpp"

void SoundOutputter::Output(const SoundObject& soundObject) const
{
	this->formattedOutputter.OutputProperty(u8"voiceAssignModeFlags", soundObject.data.voiceAssignModeFlags.encodedFlags);

	for(uint32 i = 0; i < sizeof(soundObject.data.unknown1); i++)
		this->formattedOutputter.OutputProperty(u8"unknown1 " + String::Number(i), soundObject.data.unknown1[i]);

	this->formattedOutputter.OutputProperty(u8"transposeRangeBottomKey", soundObject.data.transposeRangeBottomKey);
	this->formattedOutputter.OutputProperty(u8"transposeRangeTopKey", soundObject.data.transposeRangeTopKey);

	this->formattedOutputter.OutputUnknownProperties(soundObject.data.unknown11, sizeof(soundObject.data.unknown11));

	this->formattedOutputter.OutputProperty(u8"oscillator count packed with something additional", soundObject.data.unknownPacked);

	for(uint32 i = 0; i < sizeof(soundObject.data.unknown2); i++)
		this->formattedOutputter.OutputProperty(u8"unknown2 " + String::Number(i), soundObject.data.unknown2[i]);

	this->formattedOutputter.OutputProperty(u8"maxTime", soundObject.data.maxTime);
	this->formattedOutputter.OutputProperty(u8"maxRange", soundObject.data.maxRange);

	for(const OscillatorData& oscillatorData : soundObject.data.oscillators)
		this->Output(oscillatorData);

	if(soundObject.data.drumKitData.HasValue())
		this->Output(*soundObject.data.drumKitData);
}

//Private methods
void SoundOutputter::Output(const DrumKitSoundData &drumKitSoundData) const
{
	Section section(u8"Drum kit data", this->formattedOutputter);

	this->OutputKeyTable(drumKitSoundData);
	this->OutputLayers(drumKitSoundData);
}

void SoundOutputter::Output(const OscillatorData &oscillatorData) const
{
	Section section(u8"Oscillator", this->formattedOutputter);

	{
		Section subSection(u8"OSC multisample high", this->formattedOutputter);
		this->Output(oscillatorData.high);
	}

	{
		Section subSection(u8"OSC multisample low", this->formattedOutputter);
		this->Output(oscillatorData.low);
	}

	this->formattedOutputter.OutputProperty(u8"oscTriggerModeDelay", (uint8)oscillatorData.oscTriggerModeDelay);
	this->formattedOutputter.OutputProperty(u8"velocityMultiSampleSwitchLowHigh", oscillatorData.velocityMultiSampleSwitchLowHigh);
	this->formattedOutputter.OutputProperty(u8"velocityZoneBottom", oscillatorData.velocityZoneBottom);
	this->formattedOutputter.OutputProperty(u8"velocityZoneTop", oscillatorData.velocityZoneTop);
	this->formattedOutputter.OutputProperty(u8"keyboardRangeBottomKey", oscillatorData.keyboardRangeBottomKey);
	this->formattedOutputter.OutputProperty(u8"keyboardRangeTopKey", oscillatorData.keyboardRangeTopKey);

	this->formattedOutputter.OutputProperty(u8"unknown2", oscillatorData.unknown2);

	this->formattedOutputter.OutputProperty(u8"oscTriggerMode", (uint8)oscillatorData.oscTriggerMode);
	this->formattedOutputter.OutputProperty(u8"oscOffWhenSoundControllersAreOn", oscillatorData.oscOffWhenSoundControllersAreOn);

	this->formattedOutputter.OutputUnknownProperties(oscillatorData.unknown3, sizeof(oscillatorData.unknown3));

	this->formattedOutputter.OutputProperty(u8"octaveTranspose", oscillatorData.octaveTranspose);
	this->formattedOutputter.OutputProperty(u8"transpose", oscillatorData.transpose);
	this->formattedOutputter.OutputProperty(u8"tune", oscillatorData.tune);

	this->formattedOutputter.OutputUnknownProperties(oscillatorData.unknown4, sizeof(oscillatorData.unknown4));

	this->formattedOutputter.OutputProperty(u8"scaledVelocityBottom", oscillatorData.scaledVelocityBottom);
	this->formattedOutputter.OutputProperty(u8"scaledVelocityTop", oscillatorData.scaledVelocityTop);

	this->formattedOutputter.OutputUnknownProperties(oscillatorData.unknown5, sizeof(oscillatorData.unknown5));
}

void SoundOutputter::Output(const OSCMultiSampleSettings& oscMultiSampleSettings) const
{
	this->formattedOutputter.OutputProperty(u8"Multisample ID", String::HexNumber(oscMultiSampleSettings.multiSampleId, 16));
	this->formattedOutputter.OutputProperty(u8"Multisample number", oscMultiSampleSettings.multiSampleNumber);
	this->formattedOutputter.OutputProperty(u8"source", (uint8)oscMultiSampleSettings.source);
	this->formattedOutputter.OutputProperty(u8"level", oscMultiSampleSettings.level);
	this->formattedOutputter.OutputProperty(u8"reversed", oscMultiSampleSettings.reversed);
	this->formattedOutputter.OutputProperty(u8"offset", (uint8)oscMultiSampleSettings.offset);
}

void SoundOutputter::OutputKeyTable(const DrumKitSoundData &drumKitSoundData) const
{
	Section section(u8"Key table", this->formattedOutputter);

	for(uint8 i = 0; i < 128; i++)
	{
		const auto& entry = drumKitSoundData.keyTable[i];
		Section entrySection(u8"Key table entry", this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"pitch", Pitch(i));
		this->formattedOutputter.OutputProperty(u8"index", entry.index);
		this->formattedOutputter.OutputProperty(u8"nLayers", entry.nLayers);

		this->formattedOutputter.OutputUnknownProperties(entry.unknown1, sizeof(entry.unknown1));

		for(uint8 j = 0; j < 5; j++)
			this->formattedOutputter.OutputProperty(u8"velocity sample switch " + String::Number(j), entry.velocitySampleSwitches[j]);
	}
}

void SoundOutputter::OutputLayers(const DrumKitSoundData &drumKitSoundData) const
{
	Section section(u8"Layers", this->formattedOutputter);

	for(const auto& layer : drumKitSoundData.layers)
	{
		Section layerSection(u8"Layer", this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"unknown1", layer.unknown1);

		this->formattedOutputter.OutputProperty(u8"reversed", (uint8)layer.reversed);
		this->formattedOutputter.OutputProperty(u8"unknown11", layer.unknown11);

		this->formattedOutputter.OutputProperty(u8"multiSampleNumber", layer.multiSampleNumber);
		this->formattedOutputter.OutputProperty(u8"level", layer.level);
		this->formattedOutputter.OutputProperty(u8"transpose", layer.transpose);
		this->formattedOutputter.OutputProperty(u8"tune", layer.tune);
		this->formattedOutputter.OutputProperty(u8"attack", layer.attack);
		this->formattedOutputter.OutputProperty(u8"decay", layer.decay);
		this->formattedOutputter.OutputProperty(u8"cutoff", layer.cutoff);
		this->formattedOutputter.OutputProperty(u8"resonance", layer.resonance);

		this->formattedOutputter.OutputUnknownProperties(layer.unknown12, sizeof(layer.unknown12));

		{
			Section drumSampleEqualizerSection(u8"Drum sample equalizer", this->formattedOutputter);

			this->formattedOutputter.OutputProperty(u8"enable", layer.drumSampleEqualizer.enable);
			this->formattedOutputter.OutputProperty(u8"trim", layer.drumSampleEqualizer.trim);
			this->formattedOutputter.OutputProperty(u8"lowGainTimes2", layer.drumSampleEqualizer.lowGainTimes2);
			this->formattedOutputter.OutputProperty(u8"freq", layer.drumSampleEqualizer.freq);
			this->formattedOutputter.OutputProperty(u8"midGainTimes2", layer.drumSampleEqualizer.midGainTimes2);
			this->formattedOutputter.OutputProperty(u8"highGainTimes2", layer.drumSampleEqualizer.highGainTimes2);
		}

		this->formattedOutputter.OutputUnknownProperties(layer.unknown21, sizeof(layer.unknown21));

		this->formattedOutputter.OutputProperty(u8"intensity", layer.intensity);

		this->formattedOutputter.OutputUnknownProperties(layer.unknown22, sizeof(layer.unknown22));
	}
}

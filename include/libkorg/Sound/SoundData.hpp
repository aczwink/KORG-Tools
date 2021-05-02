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
#pragma once
//Local
#include <libkorg/Pitch.hpp>
#include "OscillatorData.hpp"

namespace libKORG::Sound
{
	enum class VoiceAssignModeFlags : uint8
	{
		IsDrumKit = 0x2,
		Legato = 0x4,
		Mono = 0x8,
		HighPriority = 0x10,
		LastPriority = 0x20,
		SingleTrigger = 0x40,
		Hold = 0x80,
	};

	enum class Reversed
	{
		No = 0,
		Yes = 2
	};

	struct KeyTableEntry
	{
		uint8 index;
		uint8 nLayers;
		uint8 unknown1[7];
		uint8 velocitySampleSwitches[5];
	};

	struct LayerEntry
	{
		uint8 unknown1;
		Reversed reversed;
		uint8 unknown11;
		uint8 multiSampleNumber; //is it sample or multisample number?
		int8 level;
		int8 transpose;
		uint8 tune;
		int8 attack;
		uint8 decay;
		int8 cutoff;
		uint8 resonance;
		uint8 unknown12[2];
		struct
		{
			bool enable;
			uint8 trim;
			int8 lowGainTimes2;
			uint8 freq;
			int8 midGainTimes2;
			int8 highGainTimes2;
		} drumSampleEqualizer;
		uint8 unknown21[2];
		int8 intensity;
		uint8 unknown22[8];
	};

	struct DrumKitSoundData
	{
		KeyTableEntry keyTable[128];
		StdXX::DynamicArray<LayerEntry> layers;
	};

	struct SoundData
	{
		StdXX::Flags<VoiceAssignModeFlags> voiceAssignModeFlags;
		uint8 unknown1[5];
		Pitch transposeRangeBottomKey;
		Pitch transposeRangeTopKey;
		byte unknown11[274];
		uint8 unknownPacked;
		uint8 lowPriority;
		byte unknown2[20];
		uint16 maxTime;
		uint8 maxRange;
		StdXX::DynamicArray<OscillatorData> oscillators;
		StdXX::Optional<DrumKitSoundData> drumKitData; //only there for drum kits
	};
}
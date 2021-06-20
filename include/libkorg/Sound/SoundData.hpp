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
#include "DrumKit.hpp"

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

	struct EffectData
	{
		uint8 unknown1 = 0;
		uint8 unknown2[64] = {};
		uint8 unknown3 = 0;
		uint8 unknown4 = 0;

		//Operators
		bool operator==(const EffectData&) const = default;
	};

	struct SoundData
	{
		/**
		 * Probably a map between ROM multisample numbers.
		 * The keyboard uses this internally on import do determine the correct mutlisamples even when the sound comes from a different keyboard model.
		 */
		uint16 multiSampleMap;
		StdXX::Flags<VoiceAssignModeFlags> voiceAssignModeFlags;
		uint8 unknown1[5]; //single bytes... probably also flags?
		KeyRange transposeRange = {0, 127};
		EffectData effects[4];
		byte unknown21[3]; //single bytes...
		uint8 unknown22 = 0;
		uint8 unknown23 = 0;
		int8 unknown3 = -1;
		uint8 unknownPacked;
		uint8 lowPriority;
		byte unknown4[6] = {0, 128, 0, 0, 128, 0};
		int8 unknown5;
		uint8 unknown6;
		int8 unknown7;
		uint8 unknown8;
		uint8 unknown9;
		int8 unknown10;
		uint8 unknown11;
		int8 unknown12;
		uint8 unknown13;
		int8 unknown14;
		uint8 unknown15;
		int8 unknown16;
		uint8 unknown17;
		uint8 unknown18;
		uint16 maxTime = 15;
		uint8 maxRange = 5;
		StdXX::DynamicArray<OscillatorData> oscillators;
		StdXX::Optional<DrumKitSoundData> drumKitData; //only there for drum kits

		//Operators
		bool operator==(const SoundData&) const = default;
	};
}
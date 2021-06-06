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

namespace libKORG::Sound
{
	enum class Reversed
	{
		No = 0,
		Yes = 2
	};

	struct KeyTableEntry
	{
		uint16 index;
		uint8 nLayers;
		uint8 unknown11[4];
		uint8 unknown12 = 96;
		uint8 unknown13[2];
		uint8 velocitySampleSwitches[5] = {127, 127, 127, 127, 127};

		//Operators
		bool operator==(const KeyTableEntry&) const = default;
	};

	struct LayerEntry
	{
		uint8 unknown1;
		Reversed reversed;
		uint16 multiSampleNumber; //is it sample or multisample number?
		int8 level;
		int8 transpose;
		uint8 tune;
		int8 attack;
		uint8 decay;
		int8 cutoff;
		uint8 resonance;
		uint16 unknown13 = 8018;
		int8 unknown14 = 10;
		EqualizerData drumSampleEqualizer;
		uint8 unknown21[2] = {0, 15};
		int8 intensity = 0;
		uint64 id;

		//Operators
		bool operator==(const LayerEntry&) const = default;
		bool operator!=(const LayerEntry&) const = default;
	};

	struct DrumKitSoundData
	{
		KeyTableEntry keyTable[128];
		StdXX::DynamicArray<LayerEntry> layers;

		//Operators
		bool operator==(const DrumKitSoundData&) const = default;
	};
}
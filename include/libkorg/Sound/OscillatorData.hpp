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
	enum class Offset
	{
		Off = 0,
		First = 1,
		//TODO: rest
		NoAttack = 8,
		//TODO: rest
	};

	struct OSCMultiSampleSettings
	{
		byte unknown1[8];
		uint16 multiSampleNumber;
		byte unknown2;
		uint8 level;
		bool reversed;
		Offset offset;
	};

	enum class OSCTriggerMode
	{
		Normal = 0,
		//TODO: REST
		Random = 9,
		//TODO: REST
	};

	enum class OSCTriggerModeDelay
	{
		//TODO: 0-50ms steps in 2ms i.e. values 0-25
		//TODO: 50-200ms steps in 10ms i.e. values 25-40
		//TODO: 200-1000ms steps in 50ms i.e. values 40-56
		MS750 = 51,
		MS5000 = 96,
		//TODO: 1000-5000ms steps in 100ms i.e. values 56-96
		//TODO: KeyOff, KeyRel, NatRel
	};

	struct OscillatorData
	{
		OSCMultiSampleSettings high;
		OSCMultiSampleSettings low;
		OSCTriggerModeDelay oscTriggerModeDelay;
		uint8 velocityMultiSampleSwitchLowHigh;
		uint8 velocityZoneBottom;
		uint8 velocityZoneTop;
		Pitch keyboardRangeBottomKey;
		Pitch keyboardRangeTopKey;
		uint8 unknown2;
		OSCTriggerMode oscTriggerMode;
		bool oscOffWhenSoundControllersAreOn;
		byte unknown3[26];
		int8 octaveTranspose;
		uint8 transpose;
		int16 tune;
		byte unknown4[90];
		uint8 scaledVelocityBottom;
		uint8 scaledVelocityTop;
		uint8 unknown5[32];
	};
}
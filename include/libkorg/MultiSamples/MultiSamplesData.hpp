/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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

namespace libKORG::MultiSamples
{
	struct DrumSampleEntry
	{
		int16 unknown1;
		int16 unknown2;
		int16 unknown3;
		int16 unknown4;
		uint8 unknown5;
		int8 unknown6;
		uint8 unknown7;
		int8 unknown8;
		StdXX::String name;
		int8 unknown9;
		int8 unknown10;
		int8 unknown11;
		int8 unknown12;
		uint64 id;
	};

	enum class InterpolationMode
	{
	};

	struct KeyboardZone
	{
		int16 sampleNumber;
		Pitch to;
		Pitch originalNote;
		uint8 unknown3;
		int16 pitch;
		int8 level;
		uint8 unknown4;
		int8 unknown5;
	};

	struct MultiSampleEntry
	{
		uint16 unknown1;
		StdXX::String name;
		uint16 keyZoneBaseIndex;
		uint8 keyZoneIndex[128];
		uint8 nKeyZones;
		uint8 unknown3;
		uint64 id;
	};

	enum class SampleType
	{
		Linear_PCM_S16BE = 0,
		Compressed = 2
	};

	enum class SampleFlags
	{
		PlayBackwards = 0x80,
		UseLoopPitch = 0x100,
		OneShot = 0x200,
		TwelveDbBoost = 0x400,
		DoNotStream = 0x800,
	};

	class PackedData
	{
	public:
		//Constructors
		PackedData() = default;

		inline PackedData(uint16 rawValue) : bitfield(rawValue)
		{
		}

		//Properties
		inline StdXX::Bitfield<uint16> Encoded() const
		{
			StdXX::Bitfield<uint16> copy = this->bitfield;
			copy.SetBit(4, true); //bit (i.e. after the sampleType) -> 1bit is extended (i.e. has uint64 id)
			return copy;
		}

		inline StdXX::Flags<enum SampleFlags, uint16> SampleFlags() const
		{
			return this->bitfield.Extract(7, 9);
		}

		inline enum InterpolationMode InterpolationMode() const
		{
			return static_cast<enum InterpolationMode>(this->bitfield.Get(5, 2));
		}

		inline enum SampleType SampleType() const
		{
			return static_cast<enum SampleType>(this->bitfield.Get(0, 4));
		}

		inline void SampleType(enum SampleType newValue)
		{
			this->bitfield.Set(0, 4, (uint16)newValue);
		}

	private:
		//Members
		StdXX::Bitfield<uint16> bitfield;
	};

	struct SampleEntry
	{
		int16 unknown1;
		int16 unknown2;
		PackedData packedData;
		uint8 unknown4;
		uint32 unknown8;
		uint32 unknown9[8];
		uint8 unknown5;
		uint32 unknown10;
		uint32 unknown11;
		uint32 unknown12;
		uint32 unknown13;
		int16 compressionCoefficients[2];
		uint32 unknown14;
		StdXX::String name;
		uint64 id;
		uint8 unknown15;
		uint8 unknown16;
		uint32 unknown17;
		libKORG::Pitch originalNote;
	};

	struct MultiSamplesData
	{
		StdXX::DynamicArray<SampleEntry> sampleEntries;
		StdXX::DynamicArray<DrumSampleEntry> drumSampleEntries;
		StdXX::DynamicArray<KeyboardZone> keyboardZones;
		StdXX::DynamicArray<MultiSampleEntry> multiSampleEntries;
	};
}
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
#include <StdXX.hpp>
#include "BankObject.hpp"
#include "ProgramChangeSequence.hpp"

namespace libKORG
{
	enum class AccompanimentTrackNumber
	{
		Drum = 0,
		Percussion = 1,
		Bass = 2,
		Accompaniment1 = 3,
		Accompaniment2 = 4,
		Accompaniment3 = 5,
		Accompaniment4 = 6,
		Accompaniment5 = 7
	};

	static const AccompanimentTrackNumber AccompanimentTrackNumbers[] = {
		AccompanimentTrackNumber::Drum,
		AccompanimentTrackNumber::Percussion,
		AccompanimentTrackNumber::Bass,
		AccompanimentTrackNumber::Accompaniment1,
		AccompanimentTrackNumber::Accompaniment2,
		AccompanimentTrackNumber::Accompaniment3,
		AccompanimentTrackNumber::Accompaniment4,
		AccompanimentTrackNumber::Accompaniment5
	};

	enum class KeyboardTrackNumber
	{
		Low = 0,
		Upper1 = 1,
		Upper2 = 2,
		Upper3 = 3
	};

	static const KeyboardTrackNumber KeyboardTrackNumbers[] = {
		KeyboardTrackNumber::Low,
		KeyboardTrackNumber::Upper1,
		KeyboardTrackNumber::Upper2,
		KeyboardTrackNumber::Upper3,
	};

	struct TrackProperties
	{
		uint16 unknown_101;
		uint8 unknown_103;
		uint8 muted;
		uint16 unknown11;
		uint16 unknown111;
		uint8 unknown1;
		uint8 unknown113;
		StdXX::UniquePointer<ProgramChangeSequence> soundProgramChangeSeq;
		uint8 volume;
		int8 pan;
		int8 detune;
		int8 octaveTranspose;
		uint8 unknown3;
		uint8 unknown4;
		uint8 unknown5;
		uint8 unknown6;
		uint8 unknown7;
		uint8 unknown71;
		uint8 unknown72;
		uint8 unknown73;
		uint8 unknown8;
		uint8 unknown9;
		uint8 fxMaster1;
		uint8 fxMaster2;
		uint8 dry;
		uint8 unknown12;
		uint8 unknown13;
		uint8 pbSensitivity;
		uint8 unknown182;
		uint8 unknown181;
		int8 lowGainTimes2;
		int8 midGainTimes2;
		int8 highGainTimes2;
		uint8 unknown18;
		uint8 unknown1811;
		uint8 unknown1812;
		uint8 unknown19;
		uint8 unknown24;
		uint8 unknown23;
		uint8 unknown25;
		uint8 unknown27;
		uint8 unknown28;
		uint8 unknown26;
		uint8 unknown239;
		uint8 unknown20;
		uint8 unknown22;
		uint8 unknown226;
		uint8 unknown218;
		uint8 unknown221;
		uint8 unknown220;
		uint8 unknown217;
		uint8 unknown224;
		uint8 unknown211;
		uint8 unknown236;
		uint8 unknown212;
		uint8 unknown238;
		uint8 unknown213;
		uint8 unknown231;
		uint8 unknown21;
		uint8 unknown241;
		uint8 unknown214;
		uint8 unknown242;
		uint8 unknown234;
		uint8 unknown235;
		uint8 unknown216;
		uint8 unknown232;
		uint8 unknown233;
		uint8 unknown219;
		uint8 unknown222;
		uint8 unknown225;
		uint8 unknown223;
		uint8 unknown215;
		uint8 unknown237;
		uint8 unknown31;
	};

	struct GeneralPerformanceSettings
	{
		struct _0x07000008_chunk
		{
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			uint8 unknown411;
			uint8 unknown5;
			uint8 unknown51;
			uint8 unknown6;
			uint8 unknown7;
			uint8 unknown71;
			uint8 unknown72;
			uint8 unknown8;
			uint8 unknown9;
			uint8 unknown10;
			uint8 unknown11;
			uint8 unknown12;
			uint8 unknown13;
			uint8 unknown14;
			uint8 unknown15;
			uint8 unknown16;
			uint8 unknown17;
			uint8 unknown18;
			uint8 unknown19;
			uint8 unknown20;
			uint8 unknown21;
			uint8 unknown22;
			uint8 unknown23;
			uint8 unknown24;
			uint8 unknown251;
			uint8 unknown25;
			uint8 unknown26;
			uint8 unknown27;
			uint8 unknown28;
			uint8 unknown29;
			uint8 unknown30;
			uint8 unknown31;
			uint8 unknown32;
			uint8 unknown33;
			uint8 unknown331;
			uint8 unknown34;
			uint8 unknown35;
			uint8 unknown36;
			uint8 unknown37;
			uint8 unknown38;
			uint8 unknown39;
			uint8 unknown40;
			uint8 unknown41;
			uint8 unknown42;
			uint8 unknown43;
			uint8 unknown44;
			uint8 unknown45;
			uint8 unknown46;
			uint8 unknown47;
			uint8 unknown48;
			uint8 unknown49;
			uint8 unknown50;
			uint8 unknown52;
			uint8 unknown53;
			uint8 unknown54;
			uint8 unknown55;
			uint8 unknown56;
			uint8 unknown57;
			uint8 unknown58;
			uint8 unknown59;
			uint8 unknown591;
			uint8 unknown60;
			uint8 unknown61;
			uint8 unknown62;
			uint8 unknown63;
			uint8 unknown64;
		};

		struct _0x08000008_chunk
		{
			uint8 unknown0;
			uint8 unknown1;
			uint8 unknown2;
			uint8 unknown3;
			uint8 unknown4;
			uint8 unknown5;
			uint8 unknown6;
			uint8 unknown8;
			uint8 unknown9;
			uint8 unknown10;
			uint8 unknown7;
		};

		StdXX::StaticArray<_0x08000008_chunk, 4> _0x08000008_chunks;
	};

	struct AccompanimentSettings : public GeneralPerformanceSettings
	{
		StdXX::StaticArray<_0x07000008_chunk, 6> _0x07000008_chunks;
		StdXX::StaticArray<struct TrackProperties, 8> trackProperties;
	};

	struct KeyboardSettings : public GeneralPerformanceSettings
	{
		StdXX::String name;
		StdXX::StaticArray<_0x07000008_chunk, 4> _0x07000008_chunks;
		StdXX::StaticArray<libKORG::TrackProperties, 4> trackProperties;

		struct
		{
			uint8 unknown21;
			uint8 unknown22;
			uint8 unknown24;
			uint8 unknown23;
		} _0x_10000008_chunk;

		struct
		{
			uint8 unknown1;
			uint8 unknown21;
			uint8 unknown22;
			uint8 unknown221;
			uint8 unknown23;
			uint8 unknown25;
			uint8 unknown24;
			uint8 unknown26;
		} _0x12000108_chunk;
	};

	class Performance : public BankObject
	{
	public:
		//Constructor
		inline Performance(StdXX::StaticArray<struct TrackProperties, 8>&& accompanimentTrackProperties)
			: accompanimentTrackProperties(Move(accompanimentTrackProperties))
		{
		}

		//Inline
		inline const struct TrackProperties& GetAccompanimentTrackProperties(AccompanimentTrackNumber accompanimentTrackNumber) const
		{
			return this->accompanimentTrackProperties[static_cast<uint32>(accompanimentTrackNumber)];
		}

		void WriteData(StdXX::DataWriter &dataWriter) const override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			//dataWriter.WriteBytes(this->data.Data(), this->data.Size());
		}

	private:
		//Members
		StdXX::StaticArray<struct TrackProperties, 8> accompanimentTrackProperties;
	};
}
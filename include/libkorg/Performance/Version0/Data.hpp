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
#include <libkorg/ProgramChangeSequence.hpp>
#include "Scale.hpp"

namespace libKORG::Performance::V0
{
	struct FX
	{
		int16 unknown8;
		uint8 unknown9[64] = {};
	};

	struct FXGroup
	{
		FX fx[2];
		int16 unknown10;
		uint8 unknown11;
		uint8 unknown12;
		uint8 unknown13;
		uint8 unknown14;
	};

	struct PadSettings
	{
		uint16 unknown51 = 0;
		uint8 unknown52 = StdXX::Unsigned<uint8>::Max();
		uint8 unknown53 = 100;
		int8 unknown54 = 64;
		uint8 unknown55 = 127;
		uint8 unknown56 = 127;
		uint8 unknown57 = StdXX::Unsigned<uint8>::Max();
		uint8 unknown58 = 0;
		uint8 unknown59 = 0;
	};


	struct UnknownPart1
	{
		uint8 unknown1[9];
		uint8 unknown2[8];
		uint8 unknown3[8];
		uint8 unknown4[10];
		uint8 unknown5[10];
		uint8 unknown6[10];
	};

	struct UnknownPart3
	{
		uint8 unknown34;
		uint8 unknown35;
	};

	struct UnknownPart5
	{
		uint8 unknown42;
		uint8 unknown43;
	};

	struct UnknownPart4
	{
		uint8 unknown36;
		uint8 unknown37;
		uint8 unknown38;
		int8 unknown39;
		int8 unknown40;
		uint8 unknown41;
		UnknownPart5 unknownPart5[3];
		PadSettings padSettings[4];
	};

	struct TrackSettings
	{
		uint32 unknown12;
		uint8 unknown13;
		uint8 unknown14;
		uint8 unknown15;
		uint8 unknown16;
		ProgramChangeSequence soundProgramChangeSeq;
		uint8 unknown18;
		int8 unknown19;
		uint8 unknown20;
		int8 unknown21;
		uint8 unknown22[16];
		uint32 unknown23;
		uint8 unknown24;
	};

	struct KeyboardSettings
	{
		uint8 unknown31;
		uint8 unknown32;
		uint8 unknown33;
		UnknownPart3 unknownPart3;
		UnknownPart4 unknownPart4;
		ScaleSettings scaleSettings;
		FXGroup unknown8;
		TrackSettings trackSettings[4];
	};

	struct UnknownPart12
	{
		uint8 unknown11;
		uint8 unknown111;
		uint8 unknown112;
	};

	struct UnknownPart11
	{
		uint8 unknown10;
		UnknownPart12 unknownPart12[2];
		int32 unknown12;
	};

	struct UnknownPart13
	{
		int32 unknown20;
		int32 unknown21;
		uint32 unknown22;
		uint32 unknown23;
		uint32 unknown24;
		int32 unknown25;
		int32 unknown26;
		int32 unknown27;
		int32 unknown28;
	};

	struct UnknownPart14
	{
		uint32 unknown30;
		uint8 unknown31[14];
	};

	struct UnknownPart17
	{
		uint8 unknown50;
		int8 unknown51[33];
		int32 unknown52;
	};

	struct UnknownPart18
	{
		int32 unknown53;
		uint8 unknown54;
		uint32 unknown55;
	};

	struct UnknownPart19
	{
		int32 unknown56;
		uint8 unknown57;
		uint8 unknown58;
		uint8 unknown59;
		uint8 unknown60;
	};

	struct UnknownPart20
	{
		uint16 unknown70;
		uint8 unknown71[8];
	};

	struct UnknownPart21
	{
		uint32 unknown80;
		uint8 unknown81[14];
	};

	struct UnknownPart16
	{
		UnknownPart17 unknownPart17;
		UnknownPart18 unknownPart18;
		UnknownPart19 unknownPart19;
		UnknownPart20 unknownPart20[4];
		UnknownPart21 unknownPart21[4];
	};

	struct _0x02000108_chunk
	{
		uint32 unknown01;
		uint32 unknown02;
		uint32 unknown03;
		uint32 unknown04;
		uint8 nKeyboardSettings;
		uint8 unknown06;
		UnknownPart11 unknownPart11;
		UnknownPart13 unknownPart13;
		UnknownPart14 unknownPart14[8];
		PadSettings unknownPart15[4];
		UnknownPart16 unknownPart16[4];
		uint32 unknownEnd;
	};

	struct AccompanimentSettings
	{
		TrackSettings trackSettings[8];
	};

	struct GeneralData
	{
		uint32 unknown1;
		uint32 unknown2;
		uint32 unknown3;
		uint8 unknown4;
		UnknownPart1 unknownPart1;
		int8 unknown7;
		FXGroup unknown8;
		AccompanimentSettings accompanimentSettings;
		uint32 unknown99;
		_0x02000108_chunk _0x02000108_data;

		//Constructors
		GeneralData() = default;
		GeneralData(const GeneralData&) = default;
		GeneralData(GeneralData&&) = default;
	};

	struct PerformanceData : public GeneralData
	{
		KeyboardSettings keyboardSettings;

		//Constructors
		PerformanceData() = default;

		inline PerformanceData(GeneralData&& generalData, KeyboardSettings&& keyboardSettings)
				: GeneralData(StdXX::Move(generalData)),
				  keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}
	};

	struct STSData : public GeneralData
	{
		StdXX::StaticArray<KeyboardSettings, 4> keyboardSettings;

		//Constructors
		inline STSData(GeneralData&& generalData, StdXX::StaticArray<KeyboardSettings, 4>&& keyboardSettings)
				: GeneralData(StdXX::Move(generalData)),
				  keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}
	};
}
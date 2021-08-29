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
#include "PerformanceV0Outputter.hpp"
//Namespaces
using namespace libKORG::Performance::V0;

//Public methods
void PerformanceV0Outputter::Output(const PerformanceData &performance)
{
	this->formattedOutputter.OutputProperty(u8"unknown1", performance.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", performance.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", performance.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", performance.unknown4);
	this->Output(performance.unknownPart1);
	this->formattedOutputter.OutputProperty(u8"unknown7", performance.unknown7);
	this->Output(performance.unknown8);

	for(uint8 i = 0; i < 8; i++)
	{
		this->Output(performance.accompanimentSettings.trackSettings[i]);
	}
	this->Output(performance.keyboardSettings);

	this->formattedOutputter.OutputProperty(u8"unknown99", performance.unknown99);
	this->Output(performance._0x02000108_data);
}

void PerformanceV0Outputter::Output(const ScaleSettings &scaleSettings)
{
	Section section(u8"ScaleSettings", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"scaleType", (uint8)scaleSettings.scaleType);
	this->formattedOutputter.OutputProperty(u8"key", (uint8)scaleSettings.key);
	for(uint32 i = 0; i < sizeof(scaleSettings.pitchTune); i++)
		this->formattedOutputter.OutputProperty(u8"pitchTune_" + String::Number(i), scaleSettings.pitchTune[i]);
}

//Private methods
void PerformanceV0Outputter::Output(const _0x02000108_chunk &chunk)
{
	Section section(u8"_0x02000108_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown01", chunk.unknown01);
	this->formattedOutputter.OutputProperty(u8"unknown02", chunk.unknown02);
	this->formattedOutputter.OutputProperty(u8"unknown03", chunk.unknown03);
	this->formattedOutputter.OutputProperty(u8"unknown04", chunk.unknown04);
	this->formattedOutputter.OutputProperty(u8"unknown06", chunk.unknown06);
	this->Output(chunk.unknownPart11);
	this->Output(chunk.unknownPart13);

	for(uint8 i = 0; i < 8; i++)
	{
		this->Output(chunk.unknownPart14[i]);
	}

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(chunk.unknownPart15[i]);
	}

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(chunk.unknownPart16[i]);
	}

	this->formattedOutputter.OutputProperty(u8"unknownEnd", chunk.unknownEnd);
}

void PerformanceV0Outputter::Output(const FX &fx)
{
	Section section(u8"FX", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown8", fx.unknown8);
	this->formattedOutputter.OutputUnknownProperties(fx.unknown9, sizeof(fx.unknown9));
}

void PerformanceV0Outputter::Output(const FXGroup &fxGroup)
{
	Section section(u8"FXGroup", this->formattedOutputter);

	for(uint8 i = 0; i < 2; i++)
	{
		this->Output(fxGroup.fx[i]);
	}

	this->formattedOutputter.OutputProperty(u8"unknown10", fxGroup.unknown10);
	this->formattedOutputter.OutputProperty(u8"unknown11", fxGroup.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown12", fxGroup.unknown12);
	this->formattedOutputter.OutputProperty(u8"unknown13", fxGroup.unknown13);
	this->formattedOutputter.OutputProperty(u8"unknown14", fxGroup.unknown14);
}

void PerformanceV0Outputter::Output(const PadSettings &padSettings)
{
	Section section(u8"PadSettings", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown51", padSettings.unknown51);
	this->formattedOutputter.OutputProperty(u8"unknown52", padSettings.unknown52);
	this->formattedOutputter.OutputProperty(u8"unknown53", padSettings.unknown53);
	this->formattedOutputter.OutputProperty(u8"unknown54", padSettings.unknown54);
	this->formattedOutputter.OutputProperty(u8"unknown55", padSettings.unknown55);
	this->formattedOutputter.OutputProperty(u8"unknown56", padSettings.unknown56);
	this->formattedOutputter.OutputProperty(u8"unknown57", padSettings.unknown57);
	this->formattedOutputter.OutputProperty(u8"unknown58", padSettings.unknown58);
	this->formattedOutputter.OutputProperty(u8"unknown59", padSettings.unknown59);
}

void PerformanceV0Outputter::Output(const UnknownPart1 &data)
{
	Section section(u8"UnknownPart1", this->formattedOutputter);

	this->formattedOutputter.OutputUnknownProperties(data.unknown1, sizeof(data.unknown1));
	this->formattedOutputter.OutputUnknownProperties(data.unknown2, sizeof(data.unknown2));
	this->formattedOutputter.OutputUnknownProperties(data.unknown3, sizeof(data.unknown3));
	this->formattedOutputter.OutputUnknownProperties(data.unknown4, sizeof(data.unknown4));
	this->formattedOutputter.OutputUnknownProperties(data.unknown5, sizeof(data.unknown5));
	this->formattedOutputter.OutputUnknownProperties(data.unknown6, sizeof(data.unknown6));
}

void PerformanceV0Outputter::Output(const KeyboardSettings &data)
{
	Section section(u8"KeyboardSettings", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown31", data.unknown31);
	this->formattedOutputter.OutputProperty(u8"unknown32", data.unknown32);
	this->formattedOutputter.OutputProperty(u8"unknown33", data.unknown33);
	this->Output(data.unknownPart3);
	this->Output(data.unknownPart4);
	this->Output(data.scaleSettings);
	this->Output(data.unknown8);

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(data.trackSettings[i]);
	}
}

void PerformanceV0Outputter::Output(const UnknownPart3 &data)
{
	Section section(u8"UnknownPart3", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown34", data.unknown34);
	this->formattedOutputter.OutputProperty(u8"unknown35", data.unknown35);
}

void PerformanceV0Outputter::Output(const UnknownPart4 &data)
{
	Section section(u8"UnknownPart4", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown36", data.unknown36);
	this->formattedOutputter.OutputProperty(u8"unknown37", data.unknown37);
	this->formattedOutputter.OutputProperty(u8"unknown38", data.unknown38);
	this->formattedOutputter.OutputProperty(u8"unknown39", data.unknown39);
	this->formattedOutputter.OutputProperty(u8"unknown40", data.unknown40);
	this->formattedOutputter.OutputProperty(u8"unknown41", data.unknown41);

	for(uint8 i = 0; i < 3; i++)
	{
		this->Output(data.unknownPart5[i]);
	}

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(data.padSettings[i]);
	}
}

void PerformanceV0Outputter::Output(const UnknownPart5 &data)
{
	Section section(u8"UnknownPart5", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown42", data.unknown42);
	this->formattedOutputter.OutputProperty(u8"unknown43", data.unknown43);
}

/*void PerformanceV0Outputter::Output(const UnknownPart7 &data)
{
	Section section(u8"UnknownPart7", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown61", data.unknown61);
	this->formattedOutputter.OutputProperty(u8"unknown62", data.unknown62);
	this->formattedOutputter.OutputUnknownProperties(data.unknown63, sizeof(data.unknown63));
}*/

void PerformanceV0Outputter::Output(const TrackSettings &data)
{
	Section section(u8"TrackSettings", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown12", data.unknown12);
	this->formattedOutputter.OutputProperty(u8"unknown13", data.unknown13);
	this->formattedOutputter.OutputProperty(u8"unknown14", data.unknown14);
	this->formattedOutputter.OutputProperty(u8"unknown15", data.unknown15);
	this->formattedOutputter.OutputProperty(u8"unknown16", data.unknown16);
	this->formattedOutputter.OutputProperty(u8"soundProgramChangeSeq", data.soundProgramChangeSeq.ToString());
	this->formattedOutputter.OutputProperty(u8"unknown18", data.unknown18);
	this->formattedOutputter.OutputProperty(u8"unknown19", data.unknown19);
	this->formattedOutputter.OutputProperty(u8"unknown20", data.unknown20);
	this->formattedOutputter.OutputProperty(u8"unknown21", data.unknown21);
	this->formattedOutputter.OutputUnknownProperties(data.unknown22, sizeof(data.unknown22));
	this->formattedOutputter.OutputProperty(u8"unknown23", data.unknown23);
	this->formattedOutputter.OutputProperty(u8"unknown24", data.unknown24);
}

void PerformanceV0Outputter::Output(const UnknownPart11 &data)
{
	Section section(u8"UnknownPart11", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown10", data.unknown10);

	for(uint8 i = 0; i < 2; i++)
	{
		this->Output(data.unknownPart12[i]);
	}

	this->formattedOutputter.OutputProperty(u8"unknown12", data.unknown12);
}

void PerformanceV0Outputter::Output(const UnknownPart12 &data)
{
	Section section(u8"UnknownPart12", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown11", data.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown111", data.unknown111);
	this->formattedOutputter.OutputProperty(u8"unknown112", data.unknown112);
}

void PerformanceV0Outputter::Output(const UnknownPart13 &data)
{
	Section section(u8"UnknownPart13", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown20", data.unknown20);
	this->formattedOutputter.OutputProperty(u8"unknown21", data.unknown21);
	this->formattedOutputter.OutputProperty(u8"unknown22", data.unknown22);
	this->formattedOutputter.OutputProperty(u8"unknown23", data.unknown23);
	this->formattedOutputter.OutputProperty(u8"unknown24", data.unknown24);
	this->formattedOutputter.OutputProperty(u8"unknown25", data.unknown25);
	this->formattedOutputter.OutputProperty(u8"unknown26", data.unknown26);
	this->formattedOutputter.OutputProperty(u8"unknown27", data.unknown27);
	this->formattedOutputter.OutputProperty(u8"unknown28", data.unknown28);
}

void PerformanceV0Outputter::Output(const UnknownPart14 &data)
{
	Section section(u8"UnknownPart14", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown30", data.unknown30);
	this->formattedOutputter.OutputUnknownProperties(data.unknown31, sizeof(data.unknown31));
}

void PerformanceV0Outputter::Output(const UnknownPart16 &data)
{
	Section section(u8"UnknownPart16", this->formattedOutputter);

	this->Output(data.unknownPart17);
	this->Output(data.unknownPart18);
	this->Output(data.unknownPart19);

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(data.unknownPart20[i]);
	}

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(data.unknownPart21[i]);
	}
}

void PerformanceV0Outputter::Output(const UnknownPart17 &data)
{
	Section section(u8"UnknownPart17", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown50", data.unknown50);
	for(uint32 i = 0; i < sizeof(data.unknown51); i++)
		this->formattedOutputter.OutputProperty(u8"unknown51_" + String::Number(i), data.unknown51[i]);
	this->formattedOutputter.OutputProperty(u8"unknown52", data.unknown52);
}

void PerformanceV0Outputter::Output(const UnknownPart18 &data)
{
	Section section(u8"UnknownPart18", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown53", data.unknown53);
	this->formattedOutputter.OutputProperty(u8"unknown54", data.unknown54);
	this->formattedOutputter.OutputProperty(u8"unknown55", data.unknown55);
}

void PerformanceV0Outputter::Output(const UnknownPart19 &data)
{
	Section section(u8"UnknownPart19", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown56", data.unknown56);
	this->formattedOutputter.OutputProperty(u8"unknown57", data.unknown57);
	this->formattedOutputter.OutputProperty(u8"unknown58", data.unknown58);
	this->formattedOutputter.OutputProperty(u8"unknown59", data.unknown59);
	this->formattedOutputter.OutputProperty(u8"unknown60", data.unknown60);
}

void PerformanceV0Outputter::Output(const UnknownPart20 &data)
{
	Section section(u8"UnknownPart20", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown70", data.unknown70);
	this->formattedOutputter.OutputUnknownProperties(data.unknown71, sizeof(data.unknown71));
}

void PerformanceV0Outputter::Output(const UnknownPart21 &data)
{
	Section section(u8"UnknownPart21", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown80", data.unknown80);
	this->formattedOutputter.OutputUnknownProperties(data.unknown81, sizeof(data.unknown81));
}
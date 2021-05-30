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
#include "PerformanceV1Outputter.hpp"
#include "PerformanceV0Outputter.hpp"
//Namespaces
using namespace libKORG::Performance::V1;

//Public methods
void PerformanceV1Outputter::Output(const _0x06000008_chunk &chunk)
{
	Section section(u8"_0x06000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
}

void PerformanceV1Outputter::Output(const _0x10000008_chunk &chunk)
{
	Section section(u8"_0x10000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", chunk.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", chunk.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", chunk.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown6", chunk.unknown6);
}

void PerformanceV1Outputter::Output(const _0x11000008_chunk &chunk)
{
	Section section(u8"_0x11000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
}

void PerformanceV1Outputter::Output(const _0x12000108_chunk &chunk)
{
	Section section(u8"_0x12000108_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputUnknownProperties(chunk.unknown1, sizeof(chunk.unknown1));
	for(uint32 i = 0; i < sizeof(chunk.unknown2); i++)
		this->formattedOutputter.OutputProperty(u8"unknown2_" + String::Number(i), chunk.unknown2[i]);
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown3, sizeof(chunk.unknown3));
}

void PerformanceV1Outputter::Output(const _0x14000008_chunk &chunk)
{
	Section section(u8"_0x14000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputUnknownProperties(chunk.unknown, sizeof(chunk.unknown));
}

void PerformanceV1Outputter::Output(const _0x15000108_chunk &chunk)
{
	Section section(u8"_0x15000108_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown", chunk.unknown);
}

void PerformanceV1Outputter::Output(const _0x18000008_chunk &chunk)
{
	Section section(u8"_0x18000008_chunk", this->formattedOutputter);

	for(uint32 i = 0; i < sizeof(chunk.unknown1); i++)
		this->formattedOutputter.OutputProperty(u8"unknown1_" + String::Number(i), chunk.unknown1[i]);
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown2, sizeof(chunk.unknown2));
}

void PerformanceV1Outputter::Output(const _0x1A000008_chunk& chunk)
{
	Section section(u8"_0x1A000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
}

void PerformanceV1Outputter::Output(const _0x1B000008_chunk& chunk)
{
	Section section(u8"_0x1B000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", chunk.unknown3);
}

void PerformanceV1Outputter::Output(const PerformanceData &performance)
{
	this->Output(performance._0x04000108_data);
	this->Output(performance.accompanimentSettings);
	this->Output(performance._0x1B000008_data);
	this->Output(performance._0x1A000008_data);
	this->Output(performance.keyboardSettings);
}

void PerformanceV1Outputter::Output(const STSData &sts)
{
	this->Output(sts._0x04000108_data);
	this->Output(sts.accompanimentSettings);
	this->Output(sts._0x1B000008_data);
	this->Output(sts._0x1A000008_data);

	for(uint8 i = 0; i < 4; i++)
	{
		const auto& stsi = sts.keyboardSettings[i];

		Section stsSection(u8"STS" + String::Number(i+1), this->formattedOutputter);

		this->Output(stsi);
	}
}

void PerformanceV1Outputter::Output(const TrackSettings &trackProperties)
{
	this->formattedOutputter.OutputProperty(u8"unknown1", trackProperties.unknown1);
	this->formattedOutputter.OutputUnknownProperties(trackProperties.unknown2, sizeof(trackProperties.unknown2));
	this->formattedOutputter.OutputProperty(u8"Sound program change sequence", trackProperties.soundProgramChangeSeq.ToString());
	this->formattedOutputter.OutputProperty(u8"volume", trackProperties.volume);
	this->formattedOutputter.OutputProperty(u8"pan", trackProperties.pan);
	this->formattedOutputter.OutputProperty(u8"detune", trackProperties.detune);
	this->formattedOutputter.OutputProperty(u8"octaveTranspose", trackProperties.octaveTranspose);
	this->formattedOutputter.OutputUnknownProperties(trackProperties.unknown3, sizeof(trackProperties.unknown3));
	this->formattedOutputter.OutputProperty(u8"fxMaster1", trackProperties.fxMaster1);
	this->formattedOutputter.OutputProperty(u8"fxMaster2", trackProperties.fxMaster2);
	this->formattedOutputter.OutputProperty(u8"dry", trackProperties.dry);
	this->formattedOutputter.OutputProperty(u8"unknown12", trackProperties.unknown12);
	this->formattedOutputter.OutputProperty(u8"pbSensitivity", trackProperties.pbSensitivity);
	this->formattedOutputter.OutputProperty(u8"unknown182", trackProperties.unknown182);
	this->formattedOutputter.OutputProperty(u8"unknown181", trackProperties.unknown181);
	this->formattedOutputter.OutputProperty(u8"lowGainTimes2", trackProperties.lowGainTimes2);
	this->formattedOutputter.OutputProperty(u8"midGainTimes2", trackProperties.midGainTimes2);
	this->formattedOutputter.OutputProperty(u8"highGainTimes2", trackProperties.highGainTimes2);
	this->formattedOutputter.OutputUnknownProperties(trackProperties.unknown6, sizeof(trackProperties.unknown6));
	for(uint32 i = 0; i < 4; i++)
		this->formattedOutputter.OutputProperty(u8"unknown4_" + String::Number(i), trackProperties.unknown4[i]);
	this->formattedOutputter.OutputUnknownProperties(reinterpret_cast<const byte *>(trackProperties.unknown5), sizeof(trackProperties.unknown5));
}

//Private methods
void PerformanceV1Outputter::Output(const _0x04000108_chunk& chunk)
{
	Section section(u8"_0x04000108_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", chunk.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", chunk.unknown4);
	this->formattedOutputter.OutputProperty(u8"metronomeTempo", chunk.metronomeTempo);
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown41, sizeof(chunk.unknown41));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown5, sizeof(chunk.unknown5));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown6, sizeof(chunk.unknown6));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown7, sizeof(chunk.unknown7));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown8, sizeof(chunk.unknown8));
}

void PerformanceV1Outputter::Output(const AccompanimentSettings &accompanimentSettings)
{
	this->Output(accompanimentSettings._0x06000008_data);

	for(uint8 i = 0; i < 6; i++)
	{
		Section section(u8"FX " + String::Number(i+1), this->formattedOutputter);

		this->Output(accompanimentSettings.fx[i]);
	}

	for(uint8 i = 0; i < 4; i++)
	{
		Section section(u8"Pad " + String::Number(i+1), this->formattedOutputter);

		this->Output(accompanimentSettings.padSettings[i]);
	}

	for(AccompanimentTrackNumber accompanimentTrackNumber : AccompanimentTrackNumbers)
	{
		Section acc(AccompanimentTrackNumberToAbbreviatedString(accompanimentTrackNumber) + u8" Track", this->formattedOutputter);

		this->Output(accompanimentSettings.trackSettings[static_cast<uint32>(accompanimentTrackNumber)]);
	}
}

void PerformanceV1Outputter::Output(const FXData &fxData)
{
	this->formattedOutputter.OutputProperty(u8"unknown2", fxData.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", fxData.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", fxData.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", fxData.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown6", fxData.unknown6);
	this->formattedOutputter.OutputProperty(u8"unknown7", fxData.unknown7);
	this->formattedOutputter.OutputProperty(u8"unknown8", fxData.unknown8);
	this->formattedOutputter.OutputProperty(u8"unknown9", fxData.unknown9);
	this->formattedOutputter.OutputProperty(u8"unknown10", fxData.unknown10);
	this->formattedOutputter.OutputUnknownProperties(fxData.unknown11, sizeof(fxData.unknown11));
}

void PerformanceV1Outputter::Output(const KeyboardSettings &keyboardSettings)
{
	this->formattedOutputter.OutputProperty(u8"Name", keyboardSettings.name);
	this->formattedOutputter.OutputProperty(u8"unknown1", keyboardSettings.unknown1);

	this->Output(keyboardSettings._0x10000008_data);
	this->Output(keyboardSettings._0x11000008_data);
	this->Output(keyboardSettings._0x12000108_data);

	PerformanceV0Outputter performanceV0Outputter(this->formattedOutputter);
	performanceV0Outputter.Output(keyboardSettings.scaleSettings);

	this->Output(keyboardSettings._0x14000008_data);

	for(uint8 i = 0; i < 4; i++)
	{
		Section section(u8"FX " + String::Number(i+1), this->formattedOutputter);

		this->Output(keyboardSettings.fx[i]);
	}

	this->Output(keyboardSettings._0x15000108_data);

	for(KeyboardTrackNumber keyboardTrackNumber : KeyboardTrackNumbers)
	{
		Section acc(KeyboardTrackNumberToAbbreviatedString(keyboardTrackNumber) + u8" Track", this->formattedOutputter);

		this->Output(keyboardSettings.trackSettings[static_cast<uint32>(keyboardTrackNumber)]);
	}

	this->Output(keyboardSettings._0x18000008_data);
}

void PerformanceV1Outputter::Output(const PadSettings& padSettings)
{
	this->formattedOutputter.OutputProperty(u8"unknown2", padSettings.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", padSettings.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", padSettings.unknown4);
	this->formattedOutputter.OutputUnknownProperties(padSettings.unknown5, sizeof(padSettings.unknown5));
	for(uint32 i = 0; i < sizeof(padSettings.unknown6); i++)
		this->formattedOutputter.OutputProperty(u8"unknown6_" + String::Number(i), padSettings.unknown6[i]);
	this->formattedOutputter.OutputProperty(u8"unknown7", padSettings.unknown7);
}
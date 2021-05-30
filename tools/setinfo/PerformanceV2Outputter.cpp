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
#include "PerformanceV2Outputter.hpp"
//Local
#include "PerformanceV1Outputter.hpp"
#include "PerformanceV0Outputter.hpp"
//Namespaces
using namespace libKORG::Performance::V2;

//Public methods
void PerformanceV2Outputter::Output(const PerformanceData &performance)
{
	this->Output(performance._0x04020008_data);
	this->Output(performance._0x20000008_data);
	this->Output(performance.accompanimentSettings);

	PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);
	performanceV1Outputter.Output(performance._0x1B000008_data);
	performanceV1Outputter.Output(performance._0x1A000008_data);

	this->Output(performance._0x21000008_data);
	this->Output(performance.keyboardSettings);
}

void PerformanceV2Outputter::Output(const STSData &sts)
{
	this->Output(sts._0x04020008_data);
	this->Output(sts._0x20000008_data);
	this->Output(sts.accompanimentSettings);

	PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);
	performanceV1Outputter.Output(sts._0x1B000008_data);
	performanceV1Outputter.Output(sts._0x1A000008_data);

	this->Output(sts._0x21000008_data);

	for(uint8 i = 0; i < 4; i++)
	{
		const auto& stsi = sts.keyboardSettings[i];

		Section stsSection(u8"STS" + String::Number(i+1), this->formattedOutputter);

		this->Output(stsi);
	}
}

//Private methods
void PerformanceV2Outputter::Output(const _0x04020008_chunk &chunk)
{
	Section section(u8"_0x04020008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->Output(chunk.subChunk);
	this->formattedOutputter.OutputProperty(u8"unknown81", chunk.unknown81);
	this->formattedOutputter.OutputProperty(u8"metronomeTempo", chunk.metronomeTempo);
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown83, sizeof(chunk.unknown83));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown9, sizeof(chunk.unknown9));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown10, sizeof(chunk.unknown10));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown11, sizeof(chunk.unknown11));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown12, sizeof(chunk.unknown12));
}

void PerformanceV2Outputter::Output(const _0x20000008_chunk &chunk)
{
	Section section(u8"_0x20000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->Output(chunk.subChunk);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", chunk.unknown3);
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown4, sizeof(chunk.unknown4));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown5, sizeof(chunk.unknown5));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown6, sizeof(chunk.unknown6));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown11, sizeof(chunk.unknown11));
	this->formattedOutputter.OutputUnknownProperties(chunk.unknown12, sizeof(chunk.unknown12));
}

void PerformanceV2Outputter::Output(const _0x21000008_chunk &chunk)
{
	Section section(u8"_0x21000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", chunk.unknown3);
}

void PerformanceV2Outputter::Output(const _0x22000008_chunk &chunk)
{
	Section section(u8"_0x22000008_chunk", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", chunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", chunk.unknown2);
}

void PerformanceV2Outputter::Output(const AccompanimentSettings &accompanimentSettings)
{
	PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);
	performanceV1Outputter.Output(accompanimentSettings._0x06000008_data);

	for(uint8 i = 0; i < 3; i++)
	{
		Section section(u8"Master FX " + String::Number(i+1), this->formattedOutputter);

		this->Output(accompanimentSettings.masterFxSettings[i]);
	}

	for(uint8 j = 0; j < 8; j++)
	{
		for (uint8 i = 0; i < 2; i++)
		{
			Section section(u8"Insert FX " + String::Number(j) + u8" - " + String::Number(i + 1), this->formattedOutputter);

			this->Output(accompanimentSettings.insertFXSettings[j][i]);
		}
	}

	for(AccompanimentTrackNumber accompanimentTrackNumber : AccompanimentTrackNumbers)
	{
		Section acc(AccompanimentTrackNumberToAbbreviatedString(accompanimentTrackNumber) + u8" Track", this->formattedOutputter);

		this->Output(accompanimentSettings.trackSettings[static_cast<uint32>(accompanimentTrackNumber)]);
	}
}

void PerformanceV2Outputter::Output(const InsertFXSettings &insertFXSettings)
{
	this->Output((const MasterFXSettings&)insertFXSettings);

	this->formattedOutputter.OutputProperty(u8"unknown6", insertFXSettings.unknown6);
}

void PerformanceV2Outputter::Output(const KeyboardSettings &keyboardSettings)
{
	this->formattedOutputter.OutputProperty(u8"Name", keyboardSettings.name);
	this->formattedOutputter.OutputProperty(u8"unknown1", keyboardSettings.unknown1);

	PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);

	performanceV1Outputter.Output(keyboardSettings._0x10000008_data);
	performanceV1Outputter.Output(keyboardSettings._0x11000008_data);
	performanceV1Outputter.Output(keyboardSettings._0x12000108_data);

	PerformanceV0Outputter performanceV0Outputter(this->formattedOutputter);
	performanceV0Outputter.Output(keyboardSettings.scaleSettings);

	for(uint8 i = 0; i < 2; i++)
	{
		Section section(u8"Master FX " + String::Number(i+1), this->formattedOutputter);

		this->Output(keyboardSettings.masterFxSettings[i]);
	}

	performanceV1Outputter.Output(keyboardSettings._0x14000008_data);
	performanceV1Outputter.Output(keyboardSettings._0x15000108_data);

	for(KeyboardTrackNumber keyboardTrackNumber : KeyboardTrackNumbers)
	{
		Section acc(KeyboardTrackNumberToAbbreviatedString(keyboardTrackNumber) + u8" Track", this->formattedOutputter);

		this->Output(keyboardSettings.trackSettings[static_cast<uint32>(keyboardTrackNumber)]);
	}

	this->Output(keyboardSettings._0x22000008_data);
	performanceV1Outputter.Output(keyboardSettings._0x18000008_data);
}

void PerformanceV2Outputter::Output(const MasterFXSettings &masterFxSettings)
{
	this->formattedOutputter.OutputProperty(u8"unknown1", masterFxSettings.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", masterFxSettings.unknown2);
	this->formattedOutputter.OutputUnknownProperties(masterFxSettings.unknown3, sizeof(masterFxSettings.unknown3));
	this->formattedOutputter.OutputProperty(u8"unknown4", masterFxSettings.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", masterFxSettings.unknown5);
}

void PerformanceV2Outputter::Output(const TrackSettings &trackSettings)
{
	PerformanceV1Outputter performanceV1Outputter(this->formattedOutputter);

	performanceV1Outputter.Output(trackSettings);

	for(uint8 i = 0; i < 2; i++)
	{
		this->Output(trackSettings.insertFxSettings[i]);
	}
}

void PerformanceV2Outputter::Output(const UnknownSubChunk &unknownSubChunk)
{
	this->formattedOutputter.OutputProperty(u8"unknown1", unknownSubChunk.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", unknownSubChunk.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", unknownSubChunk.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", unknownSubChunk.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", unknownSubChunk.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown6", unknownSubChunk.unknown6);

	for(uint32 i = 0; i < unknownSubChunk.unknown7.GetNumberOfElements(); i++)
	{
		this->formattedOutputter.OutputProperty(u8"unknown7_" + String::Number(i), unknownSubChunk.unknown7[i]);
	}
}

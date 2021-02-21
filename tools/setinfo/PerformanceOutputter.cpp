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
#include "PerformanceOutputter.hpp"

//Public methods
void PerformanceOutputter::Output(const Performance &performance)
{
	this->Output(performance.AccompanimentSettings());
	this->Output(performance.KeyboardSettings());
}

void PerformanceOutputter::Output(const SingleTouchSettings &sts)
{
	Section section(u8"Single Touch Settings", this->formattedOutputter);

	this->Output(sts.AccompanimentSettings());

	for(uint8 i = 0; i < 4; i++)
	{
		const auto& stsi = sts.GetSTS(i);

		Section stsSection(u8"STS" + String::Number(i+1), this->formattedOutputter);

		this->Output(stsi);
	}
}

//Private methods
void PerformanceOutputter::Output(const AccompanimentSettings &accompanimentSettings)
{
	this->Output(accompanimentSettings.trackProperties);

	for(const auto& chunk : accompanimentSettings.unknownChunksBefore9)
		this->formattedOutputter.OutputUnknownChunk(chunk);
}

void PerformanceOutputter::Output(const StaticArray<struct TrackProperties, 4> &keyboardTrackProperties)
{
	for(KeyboardTrackNumber keyboardTrackNumber : KeyboardTrackNumbers)
	{
		Section acc(KeyboardTrackNumberToAbbreviatedString(keyboardTrackNumber) + u8" Track", this->formattedOutputter);

		this->Output(keyboardTrackProperties[static_cast<uint32>(keyboardTrackNumber)]);
	}
}

void PerformanceOutputter::Output(const StdXX::StaticArray<struct TrackProperties, 8>& accompanimentTrackProperties)
{
	for(AccompanimentTrackNumber accompanimentTrackNumber : AccompanimentTrackNumbers)
	{
		Section acc(AccompanimentTrackNumberToAbbreviatedString(accompanimentTrackNumber) + u8" Track", this->formattedOutputter);

		this->Output(accompanimentTrackProperties[static_cast<uint32>(accompanimentTrackNumber)]);
	}
}

void PerformanceOutputter::Output(const KeyboardSettings &keyboardSettings)
{
	this->formattedOutputter.OutputProperty(u8"Name", keyboardSettings.name);

	for(const auto& chunk : keyboardSettings.unknownChunksBeforeTracks)
		this->formattedOutputter.OutputUnknownChunk(chunk);

	this->Output(keyboardSettings.trackProperties);
}

void PerformanceOutputter::Output(const TrackProperties &trackProperties)
{
	this->formattedOutputter.OutputProperty(u8"Sound program change sequence", trackProperties.soundProgramChangeSeq->ToString());
	this->formattedOutputter.OutputProperty(u8"volume", trackProperties.volume);
	this->formattedOutputter.OutputProperty(u8"pan", trackProperties.pan);
	this->formattedOutputter.OutputProperty(u8"detune", trackProperties.detune);
	this->formattedOutputter.OutputProperty(u8"octaveTranspose", trackProperties.octaveTranspose);
	this->formattedOutputter.OutputProperty(u8"fxMaster1", trackProperties.fxMaster1);
	this->formattedOutputter.OutputProperty(u8"fxMaster2", trackProperties.fxMaster2);
	this->formattedOutputter.OutputProperty(u8"dry", trackProperties.dry);
	this->formattedOutputter.OutputProperty(u8"unknown12", trackProperties.unknown12);
	this->formattedOutputter.OutputProperty(u8"unknown13", trackProperties.unknown13);
	this->formattedOutputter.OutputProperty(u8"pbSensitivity", trackProperties.pbSensitivity);
	this->formattedOutputter.OutputProperty(u8"unknown182", trackProperties.unknown182);
	this->formattedOutputter.OutputProperty(u8"unknown181", trackProperties.unknown181);
	this->formattedOutputter.OutputProperty(u8"lowGainTimes2", trackProperties.lowGainTimes2);
	this->formattedOutputter.OutputProperty(u8"midGainTimes2", trackProperties.midGainTimes2);
	this->formattedOutputter.OutputProperty(u8"highGainTimes2", trackProperties.highGainTimes2);
}
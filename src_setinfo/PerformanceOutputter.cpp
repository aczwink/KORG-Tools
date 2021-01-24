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

	for(uint8 i = 0; i < 6; i++)
	{
		this->Output(accompanimentSettings._0x07000008_chunks[i]);
	}
	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(accompanimentSettings._0x08000008_chunks[i]);
	}
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

void PerformanceOutputter::Output(const GeneralPerformanceSettings::_0x07000008_chunk &data)
{
	Section section(u8"0x07000008", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", data.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", data.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", data.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", data.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown411", data.unknown411);
	this->formattedOutputter.OutputProperty(u8"unknown412", data.unknown412);
	this->formattedOutputter.OutputProperty(u8"unknown5", data.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown51", data.unknown51);
	this->formattedOutputter.OutputProperty(u8"unknown6", data.unknown6);
	this->formattedOutputter.OutputProperty(u8"unknown7", data.unknown7);
	this->formattedOutputter.OutputProperty(u8"unknown71", data.unknown71);
	this->formattedOutputter.OutputProperty(u8"unknown72", data.unknown72);
	this->formattedOutputter.OutputProperty(u8"unknown8", data.unknown8);
	this->formattedOutputter.OutputProperty(u8"unknown9", data.unknown9);
	this->formattedOutputter.OutputProperty(u8"unknown10", data.unknown10);
	this->formattedOutputter.OutputProperty(u8"unknown11", data.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown12", data.unknown12);
	this->formattedOutputter.OutputProperty(u8"unknown13", data.unknown13);
	this->formattedOutputter.OutputProperty(u8"unknown14", data.unknown14);
	this->formattedOutputter.OutputProperty(u8"unknown15", data.unknown15);
	this->formattedOutputter.OutputProperty(u8"unknown16", data.unknown16);
	this->formattedOutputter.OutputProperty(u8"unknown17", data.unknown17);
	this->formattedOutputter.OutputProperty(u8"unknown18", data.unknown18);
	this->formattedOutputter.OutputProperty(u8"unknown19", data.unknown19);
	this->formattedOutputter.OutputProperty(u8"unknown20", data.unknown20);
	this->formattedOutputter.OutputProperty(u8"unknown21", data.unknown21);
	this->formattedOutputter.OutputProperty(u8"unknown22", data.unknown22);
	this->formattedOutputter.OutputProperty(u8"unknown23", data.unknown23);
	this->formattedOutputter.OutputProperty(u8"unknown24", data.unknown24);
	this->formattedOutputter.OutputProperty(u8"unknown251", data.unknown251);
	this->formattedOutputter.OutputProperty(u8"unknown25", data.unknown25);
	this->formattedOutputter.OutputProperty(u8"unknown26", data.unknown26);
	this->formattedOutputter.OutputProperty(u8"unknown27", data.unknown27);
	this->formattedOutputter.OutputProperty(u8"unknown28", data.unknown28);
	this->formattedOutputter.OutputProperty(u8"unknown29", data.unknown29);
	this->formattedOutputter.OutputProperty(u8"unknown30", data.unknown30);
	this->formattedOutputter.OutputProperty(u8"unknown31", data.unknown31);
	this->formattedOutputter.OutputProperty(u8"unknown32", data.unknown32);
	this->formattedOutputter.OutputProperty(u8"unknown33", data.unknown33);
	this->formattedOutputter.OutputProperty(u8"unknown331", data.unknown331);
	this->formattedOutputter.OutputProperty(u8"unknown34", data.unknown34);
	this->formattedOutputter.OutputProperty(u8"unknown35", data.unknown35);
	this->formattedOutputter.OutputProperty(u8"unknown36", data.unknown36);
	this->formattedOutputter.OutputProperty(u8"unknown37", data.unknown37);
	this->formattedOutputter.OutputProperty(u8"unknown38", data.unknown38);
	this->formattedOutputter.OutputProperty(u8"unknown39", data.unknown39);
	this->formattedOutputter.OutputProperty(u8"unknown40", data.unknown40);
	this->formattedOutputter.OutputProperty(u8"unknown41", data.unknown41);
	this->formattedOutputter.OutputProperty(u8"unknown42", data.unknown42);
	this->formattedOutputter.OutputProperty(u8"unknown43", data.unknown43);
	this->formattedOutputter.OutputProperty(u8"unknown44", data.unknown44);
	this->formattedOutputter.OutputProperty(u8"unknown45", data.unknown45);
	this->formattedOutputter.OutputProperty(u8"unknown46", data.unknown46);
	this->formattedOutputter.OutputProperty(u8"unknown47", data.unknown47);
	this->formattedOutputter.OutputProperty(u8"unknown48", data.unknown48);
	this->formattedOutputter.OutputProperty(u8"unknown49", data.unknown49);
	this->formattedOutputter.OutputProperty(u8"unknown50", data.unknown50);
	this->formattedOutputter.OutputProperty(u8"unknown52", data.unknown52);
	this->formattedOutputter.OutputProperty(u8"unknown53", data.unknown53);
	this->formattedOutputter.OutputProperty(u8"unknown54", data.unknown54);
	this->formattedOutputter.OutputProperty(u8"unknown55", data.unknown55);
	this->formattedOutputter.OutputProperty(u8"unknown56", data.unknown56);
	this->formattedOutputter.OutputProperty(u8"unknown57", data.unknown57);
	this->formattedOutputter.OutputProperty(u8"unknown58", data.unknown58);
	this->formattedOutputter.OutputProperty(u8"unknown59", data.unknown59);
	this->formattedOutputter.OutputProperty(u8"unknown591", data.unknown591);
	this->formattedOutputter.OutputProperty(u8"unknown60", data.unknown60);
	this->formattedOutputter.OutputProperty(u8"unknown61", data.unknown61);
	this->formattedOutputter.OutputProperty(u8"unknown62", data.unknown62);
	this->formattedOutputter.OutputProperty(u8"unknown63", data.unknown63);
	this->formattedOutputter.OutputProperty(u8"unknown64", data.unknown64);
}

void PerformanceOutputter::Output(const GeneralPerformanceSettings::_0x08000008_chunk &data)
{
	Section section(u8"0x08000008", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown0", data.unknown0);
	this->formattedOutputter.OutputProperty(u8"unknown1", data.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", data.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", data.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", data.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", data.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown6", data.unknown6);
	this->formattedOutputter.OutputProperty(u8"unknown8", data.unknown8);
	this->formattedOutputter.OutputProperty(u8"unknown9", data.unknown9);
	this->formattedOutputter.OutputProperty(u8"unknown10", data.unknown10);
	this->formattedOutputter.OutputProperty(u8"unknown7", data.unknown7);
}

void PerformanceOutputter::Output(const KeyboardSettings &keyboardSettings)
{
	this->formattedOutputter.OutputProperty(u8"Name", keyboardSettings.name);

	this->Output(keyboardSettings.trackProperties);

	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(keyboardSettings._0x07000008_chunks[i]);
	}
	for(uint8 i = 0; i < 4; i++)
	{
		this->Output(keyboardSettings._0x08000008_chunks[i]);
	}

	{
		Section section(u8"_0x10000008", this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"unknown21", keyboardSettings._0x_10000008_chunk.unknown21);
		this->formattedOutputter.OutputProperty(u8"unknown22", keyboardSettings._0x_10000008_chunk.unknown22);
		this->formattedOutputter.OutputProperty(u8"unknown24", keyboardSettings._0x_10000008_chunk.unknown24);
		this->formattedOutputter.OutputProperty(u8"unknown23", keyboardSettings._0x_10000008_chunk.unknown23);
	}

	{
		Section section(u8"_0x12000108", this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"unknown1", keyboardSettings._0x12000108_chunk.unknown1);
		this->formattedOutputter.OutputProperty(u8"unknown21", keyboardSettings._0x12000108_chunk.unknown21);
		this->formattedOutputter.OutputProperty(u8"unknown22", keyboardSettings._0x12000108_chunk.unknown22);
		this->formattedOutputter.OutputProperty(u8"unknown221", keyboardSettings._0x12000108_chunk.unknown221);
		this->formattedOutputter.OutputProperty(u8"unknown23", keyboardSettings._0x12000108_chunk.unknown23);
		this->formattedOutputter.OutputProperty(u8"unknown25", keyboardSettings._0x12000108_chunk.unknown25);
		this->formattedOutputter.OutputProperty(u8"unknown24", keyboardSettings._0x12000108_chunk.unknown24);
		this->formattedOutputter.OutputProperty(u8"unknown26", keyboardSettings._0x12000108_chunk.unknown26);
	}
}

void PerformanceOutputter::Output(const TrackProperties &trackProperties)
{
	this->formattedOutputter.OutputProperty(u8"unknown_101", trackProperties.unknown_101);
	this->formattedOutputter.OutputProperty(u8"unknown_103", trackProperties.unknown_103);
	this->formattedOutputter.OutputProperty(u8"muted???", trackProperties.muted);
	this->formattedOutputter.OutputProperty(u8"unknown11", trackProperties.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown111", trackProperties.unknown111);
	this->formattedOutputter.OutputProperty(u8"unknown1", trackProperties.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown113", trackProperties.unknown113);
	this->formattedOutputter.OutputProperty(u8"Sound program change sequence", trackProperties.soundProgramChangeSeq->ToString());
	this->formattedOutputter.OutputProperty(u8"volume", trackProperties.volume);
	this->formattedOutputter.OutputProperty(u8"pan", trackProperties.pan);
	this->formattedOutputter.OutputProperty(u8"detune", trackProperties.detune);
	this->formattedOutputter.OutputProperty(u8"octaveTranspose", trackProperties.octaveTranspose);
	this->formattedOutputter.OutputProperty(u8"unknown3", trackProperties.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", trackProperties.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", trackProperties.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown6", trackProperties.unknown6);
	this->formattedOutputter.OutputProperty(u8"unknown7", trackProperties.unknown7);
	this->formattedOutputter.OutputProperty(u8"unknown71", trackProperties.unknown71);
	this->formattedOutputter.OutputProperty(u8"unknown72", trackProperties.unknown72);
	this->formattedOutputter.OutputProperty(u8"unknown73", trackProperties.unknown73);
	this->formattedOutputter.OutputProperty(u8"unknown8", trackProperties.unknown8);
	this->formattedOutputter.OutputProperty(u8"unknown9", trackProperties.unknown9);
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
	this->formattedOutputter.OutputProperty(u8"unknown18", trackProperties.unknown18);
	this->formattedOutputter.OutputProperty(u8"unknown1811", trackProperties.unknown1811);
	this->formattedOutputter.OutputProperty(u8"unknown1812", trackProperties.unknown1812);
	this->formattedOutputter.OutputProperty(u8"unknown19", trackProperties.unknown19);
	this->formattedOutputter.OutputProperty(u8"unknown401", trackProperties.unknown401);
	this->formattedOutputter.OutputProperty(u8"unknown24", trackProperties.unknown24);
	this->formattedOutputter.OutputProperty(u8"unknown402", trackProperties.unknown402);
	this->formattedOutputter.OutputProperty(u8"unknown23", trackProperties.unknown23);
	this->formattedOutputter.OutputProperty(u8"unknown25", trackProperties.unknown25);
	this->formattedOutputter.OutputProperty(u8"unknown27", trackProperties.unknown27);
	this->formattedOutputter.OutputProperty(u8"unknown28", trackProperties.unknown28);
	this->formattedOutputter.OutputProperty(u8"unknown26", trackProperties.unknown26);
	this->formattedOutputter.OutputProperty(u8"unknown239", trackProperties.unknown239);
	this->formattedOutputter.OutputProperty(u8"unknown20", trackProperties.unknown20);
	this->formattedOutputter.OutputProperty(u8"unknown301", trackProperties.unknown301);
	this->formattedOutputter.OutputProperty(u8"unknown22", trackProperties.unknown22);
	this->formattedOutputter.OutputProperty(u8"unknown226", trackProperties.unknown226);
	this->formattedOutputter.OutputProperty(u8"unknown218", trackProperties.unknown218);
	this->formattedOutputter.OutputProperty(u8"unknown221", trackProperties.unknown221);
	this->formattedOutputter.OutputProperty(u8"unknown220", trackProperties.unknown220);
	this->formattedOutputter.OutputProperty(u8"unknown217", trackProperties.unknown217);
	this->formattedOutputter.OutputProperty(u8"unknown224", trackProperties.unknown224);
	this->formattedOutputter.OutputProperty(u8"unknown211", trackProperties.unknown211);
	this->formattedOutputter.OutputProperty(u8"unknown236", trackProperties.unknown236);
	this->formattedOutputter.OutputProperty(u8"unknown212", trackProperties.unknown212);
	this->formattedOutputter.OutputProperty(u8"unknown238", trackProperties.unknown238);
	this->formattedOutputter.OutputProperty(u8"unknown213", trackProperties.unknown213);
	this->formattedOutputter.OutputProperty(u8"unknown231", trackProperties.unknown231);
	this->formattedOutputter.OutputProperty(u8"unknown21", trackProperties.unknown21);
	this->formattedOutputter.OutputProperty(u8"unknown241", trackProperties.unknown241);
	this->formattedOutputter.OutputProperty(u8"unknown214", trackProperties.unknown214);
	this->formattedOutputter.OutputProperty(u8"unknown242", trackProperties.unknown242);
	this->formattedOutputter.OutputProperty(u8"unknown234", trackProperties.unknown234);
	this->formattedOutputter.OutputProperty(u8"unknown235", trackProperties.unknown235);
	this->formattedOutputter.OutputProperty(u8"unknown216", trackProperties.unknown216);
	this->formattedOutputter.OutputProperty(u8"unknown232", trackProperties.unknown232);
	this->formattedOutputter.OutputProperty(u8"unknown233", trackProperties.unknown233);
	this->formattedOutputter.OutputProperty(u8"unknown219", trackProperties.unknown219);
	this->formattedOutputter.OutputProperty(u8"unknown222", trackProperties.unknown222);
	this->formattedOutputter.OutputProperty(u8"unknown225", trackProperties.unknown225);
	this->formattedOutputter.OutputProperty(u8"unknown223", trackProperties.unknown223);
	this->formattedOutputter.OutputProperty(u8"unknown307", trackProperties.unknown307);
	this->formattedOutputter.OutputProperty(u8"unknown306", trackProperties.unknown306);
	this->formattedOutputter.OutputProperty(u8"unknown215", trackProperties.unknown215);
	this->formattedOutputter.OutputProperty(u8"unknown304", trackProperties.unknown304);
	this->formattedOutputter.OutputProperty(u8"unknown237", trackProperties.unknown237);
	this->formattedOutputter.OutputProperty(u8"unknown305", trackProperties.unknown305);
	this->formattedOutputter.OutputProperty(u8"unknown310", trackProperties.unknown310);
	this->formattedOutputter.OutputProperty(u8"unknown240", trackProperties.unknown240);
	this->formattedOutputter.OutputProperty(u8"unknown31", trackProperties.unknown31);
	this->formattedOutputter.OutputProperty(u8"unknown302", trackProperties.unknown302);
	this->formattedOutputter.OutputProperty(u8"unknown303", trackProperties.unknown303);
}
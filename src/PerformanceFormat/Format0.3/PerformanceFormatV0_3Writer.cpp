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
#include "PerformanceFormatV0_3Writer.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Performance::V0;

//Public methods
void PerformanceFormatV0_3Writer::Write(const PerformanceObject &performanceObject)
{
	this->Write0x00010008Chunk(performanceObject.V0Data());
	this->Write0x02000108Chunk(performanceObject.V0Data());
}

void PerformanceFormatV0_3Writer::Write(const libKORG::SingleTouchSettings &singleTouchSettings)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
void PerformanceFormatV0_3Writer::Write0x00010008Chunk(const Performance::V0::PerformanceData &performanceData)
{
	this->BeginChunk(0, 1, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteUInt32(performanceData.unknown1);
	this->dataWriter.WriteUInt32(performanceData.unknown2);
	this->dataWriter.WriteUInt32(performanceData.unknown3);
	this->dataWriter.WriteByte(1);
	this->dataWriter.WriteByte(performanceData.unknown4);

	this->dataWriter.WriteBytes(performanceData.unknownPart1.unknown1, sizeof(performanceData.unknownPart1.unknown1));
	this->dataWriter.WriteBytes(performanceData.unknownPart1.unknown2, sizeof(performanceData.unknownPart1.unknown2));
	this->dataWriter.WriteBytes(performanceData.unknownPart1.unknown3, sizeof(performanceData.unknownPart1.unknown3));
	this->dataWriter.WriteBytes(performanceData.unknownPart1.unknown4, sizeof(performanceData.unknownPart1.unknown4));
	this->dataWriter.WriteBytes(performanceData.unknownPart1.unknown5, sizeof(performanceData.unknownPart1.unknown5));
	this->dataWriter.WriteBytes(performanceData.unknownPart1.unknown6, sizeof(performanceData.unknownPart1.unknown6));

	this->WriteAccompanimentSettings(performanceData);
	this->WriteKeyboardSettings(performanceData.keyboardSettings);

	this->dataWriter.WriteUInt32(performanceData.unknown99);

	this->EndChunk();
}

void PerformanceFormatV0_3Writer::Write0x02000108Chunk(const Performance::V0::PerformanceData &performanceData)
{
	this->BeginChunk(2, 0, 1, ChunkHeaderFlags::Leaf);

	const auto& chunk = performanceData._0x02000108_data;

	this->dataWriter.WriteUInt32(chunk.unknown01);
	this->dataWriter.WriteUInt32(chunk.unknown02);
	this->dataWriter.WriteUInt32(chunk.unknown03);
	this->dataWriter.WriteUInt32(chunk.unknown04);
	this->dataWriter.WriteByte(chunk.nKeyboardSettings);
	this->dataWriter.WriteByte(chunk.unknown06);

	this->dataWriter.WriteByte(chunk.unknownPart11.unknown10);
	for(uint8 i = 0; i < 2; i++)
	{
		this->dataWriter.WriteByte(chunk.unknownPart11.unknownPart12[i].unknown11);
		this->dataWriter.WriteByte(chunk.unknownPart11.unknownPart12[i].unknown111);
		this->dataWriter.WriteByte(chunk.unknownPart11.unknownPart12[i].unknown112);
	}

	this->dataWriter.WriteInt32(chunk.unknownPart11.unknown12);



	this->dataWriter.WriteInt32(chunk.unknownPart13.unknown20);
	this->dataWriter.WriteInt32(chunk.unknownPart13.unknown21);
	this->dataWriter.WriteUInt32(chunk.unknownPart13.unknown22);
	this->dataWriter.WriteUInt32(chunk.unknownPart13.unknown23);
	this->dataWriter.WriteUInt32(chunk.unknownPart13.unknown24);
	this->dataWriter.WriteInt32(chunk.unknownPart13.unknown25);
	this->dataWriter.WriteInt32(chunk.unknownPart13.unknown26);
	this->dataWriter.WriteInt32(chunk.unknownPart13.unknown27);
	this->dataWriter.WriteInt32(chunk.unknownPart13.unknown28);

	for(uint8 i = 0; i < 8; i++)
	{
		auto& subChunk = chunk.unknownPart14[i];
		this->dataWriter.WriteUInt32(subChunk.unknown30);
		this->dataWriter.WriteBytes(subChunk.unknown31, sizeof(subChunk.unknown31));
	}


	for (uint8 i = 0; i < 4; i++)
		this->WritePadSettings(chunk.unknownPart15[i]);

	for(uint8 i = 0; i < chunk.nKeyboardSettings; i++)
	{
		auto& subChunk = chunk.unknownPart16[i];

		this->dataWriter.WriteByte(subChunk.unknownPart17.unknown50);
		this->dataWriter.WriteBytes(subChunk.unknownPart17.unknown51, sizeof(subChunk.unknownPart17.unknown51));
		this->dataWriter.WriteInt32(subChunk.unknownPart17.unknown52);


		this->dataWriter.WriteInt32(subChunk.unknownPart18.unknown53);
		this->dataWriter.WriteByte(subChunk.unknownPart18.unknown54);
		this->dataWriter.WriteUInt32(subChunk.unknownPart18.unknown55);




		this->dataWriter.WriteInt32(subChunk.unknownPart19.unknown56);
		this->dataWriter.WriteByte(subChunk.unknownPart19.unknown57);
		this->dataWriter.WriteByte(subChunk.unknownPart19.unknown58);
		this->dataWriter.WriteByte(subChunk.unknownPart19.unknown59);
		this->dataWriter.WriteByte(subChunk.unknownPart19.unknown60);


		for(uint8 j = 0; j < 4; j++)
		{
			this->dataWriter.WriteUInt16(subChunk.unknownPart20[j].unknown70);
			this->dataWriter.WriteBytes(subChunk.unknownPart20[j].unknown71, sizeof(subChunk.unknownPart20[j].unknown71));
		}

		for(uint8 j = 0; j < 4; j++)
		{
			this->dataWriter.WriteUInt32(subChunk.unknownPart21[j].unknown80);
			this->dataWriter.WriteBytes(subChunk.unknownPart21[j].unknown81, sizeof(subChunk.unknownPart21[j].unknown81));
		}
	}

	this->dataWriter.WriteUInt32(chunk.unknownEnd);

	this->EndChunk();
}

void PerformanceFormatV0_3Writer::WriteAccompanimentSettings(const Performance::V0::GeneralData &generalData)
{
	this->dataWriter.WriteInt8(generalData.unknown7);

	this->WriteEffectGroup(generalData.unknown8);

	for(uint8 i = 0; i < 8; i++)
	{
		this->WriteTrackSettings(generalData.unknownPart8[i]);
	}
}

void PerformanceFormatV0_3Writer::WriteEffectGroup(const Performance::V0::FXGroup &fxGroup)
{
	for(uint8 i = 0; i < 2; i++)
	{
		this->dataWriter.WriteInt16(fxGroup.fx[i].unknown8);
		this->dataWriter.WriteBytes(fxGroup.fx[i].unknown9, sizeof(fxGroup.fx[i].unknown9));
	}

	this->dataWriter.WriteInt16(fxGroup.unknown10);
	this->dataWriter.WriteByte(fxGroup.unknown11);
	this->dataWriter.WriteByte(fxGroup.unknown12);
	this->dataWriter.WriteByte(fxGroup.unknown13);
	this->dataWriter.WriteByte(fxGroup.unknown14);
}

void PerformanceFormatV0_3Writer::WriteKeyboardSettings(const KeyboardSettings &keyboardSettings)
{
	this->dataWriter.WriteByte(keyboardSettings.unknown31);
	this->dataWriter.WriteByte(keyboardSettings.unknown32);
	this->dataWriter.WriteByte(keyboardSettings.unknown33);

	this->dataWriter.WriteByte(keyboardSettings.unknownPart3.unknown34);
	this->dataWriter.WriteByte(keyboardSettings.unknownPart3.unknown35);

	this->dataWriter.WriteByte(keyboardSettings.unknownPart4.unknown36);
	this->dataWriter.WriteByte(keyboardSettings.unknownPart4.unknown37);
	this->dataWriter.WriteByte(keyboardSettings.unknownPart4.unknown38);
	this->dataWriter.WriteInt8(keyboardSettings.unknownPart4.unknown39);
	this->dataWriter.WriteInt8(keyboardSettings.unknownPart4.unknown40);
	this->dataWriter.WriteByte(keyboardSettings.unknownPart4.unknown41);
	for(uint8 j = 0; j < 3; j++)
	{
		this->dataWriter.WriteByte(keyboardSettings.unknownPart4.unknownPart5[j].unknown42);
		this->dataWriter.WriteByte(keyboardSettings.unknownPart4.unknownPart5[j].unknown43);
	}

	for(uint8 j = 0; j < 4; j++)
		this->WritePadSettings(keyboardSettings.unknownPart4.padSettings[j]);

	this->WriteScaleSettings(keyboardSettings.scaleSettings);

	this->WriteEffectGroup(keyboardSettings.unknown8);

	for(uint8 j = 0; j < 4; j++)
	{
		this->WriteTrackSettings(keyboardSettings.trackSettings[j]);
	}
}

void PerformanceFormatV0_3Writer::WritePadSettings(const PadSettings &padSettings)
{
	this->dataWriter.WriteUInt16(padSettings.unknown51);
	this->dataWriter.WriteByte(padSettings.unknown52);
	this->dataWriter.WriteByte(padSettings.unknown53);
	this->dataWriter.WriteInt8(padSettings.unknown54);
	this->dataWriter.WriteByte(padSettings.unknown55);
	this->dataWriter.WriteByte(padSettings.unknown56);
	this->dataWriter.WriteByte(padSettings.unknown57);
	this->dataWriter.WriteByte(padSettings.unknown58);
	this->dataWriter.WriteByte(padSettings.unknown59);
}

void PerformanceFormatV0_3Writer::WriteScaleSettings(const ScaleSettings &scaleSettings)
{
	this->dataWriter.WriteByte(static_cast<byte>(scaleSettings.scaleType));
	this->dataWriter.WriteByte(static_cast<byte>(scaleSettings.key));
	this->dataWriter.WriteBytes(scaleSettings.pitchTune, sizeof(scaleSettings.pitchTune));
}

void PerformanceFormatV0_3Writer::WriteTrackSettings(const TrackSettings &trackSettings)
{
	this->dataWriter.WriteUInt32(trackSettings.unknown12);
	this->dataWriter.WriteByte(trackSettings.unknown13);
	this->dataWriter.WriteByte(trackSettings.unknown14);
	this->dataWriter.WriteByte(trackSettings.unknown15);
	this->dataWriter.WriteByte(trackSettings.unknown16);

	this->dataWriter.WriteUInt32(trackSettings.soundProgramChangeSeq.ToUInt32());

	this->dataWriter.WriteByte(trackSettings.unknown18);
	this->dataWriter.WriteInt8(trackSettings.unknown19);
	this->dataWriter.WriteByte(trackSettings.unknown20);
	this->dataWriter.WriteInt8(trackSettings.unknown21);
	this->dataWriter.WriteBytes(trackSettings.unknown22, sizeof(trackSettings.unknown22));
	this->dataWriter.WriteUInt32(trackSettings.unknown23);
	this->dataWriter.WriteByte(trackSettings.unknown24);
}

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
#include "PerformanceFormatV0_2Reader.hpp"
//Local
#include <libkorg/BankFormat/PerformanceObject.hpp>
#include <libkorg/BankFormat/SingleTouchSettings.hpp>
#include "ScaleSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Performance::V0;
using namespace StdXX;

//Public methods
libKORG::BankFormat::BankObject *PerformanceFormatV0_2Reader::TakeResult()
{
	if(this->stylePerformances)
		return new SingleTouchSettings(new Performance::V0::STSData(Move(this->generalData), Move(this->keyboardSettings)));
	return new PerformanceObject(new Performance::V0::PerformanceData(Move(this->generalData), Move(this->keyboardSettings[0])));
}

//Protected methods
libKORG::ChunkReader *PerformanceFormatV0_2Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

void PerformanceFormatV0_2Reader::Read0x00000008Chunk(StdXX::DataReader &dataReader)
{
	this->generalData.unknown1 = dataReader.ReadUInt32();
	this->generalData.unknown2 = dataReader.ReadUInt32();
	this->generalData.unknown3 = dataReader.ReadUInt32();
	uint8 nKeyboardSettings = dataReader.ReadByte();
	this->generalData.unknown4 = dataReader.ReadByte();

	dataReader.ReadBytes(this->generalData.unknownPart1.unknown1, sizeof(this->generalData.unknownPart1.unknown1));
	dataReader.ReadBytes(this->generalData.unknownPart1.unknown2, sizeof(this->generalData.unknownPart1.unknown2));
	dataReader.ReadBytes(this->generalData.unknownPart1.unknown3, sizeof(this->generalData.unknownPart1.unknown3));
	dataReader.ReadBytes(this->generalData.unknownPart1.unknown4, sizeof(this->generalData.unknownPart1.unknown4));
	dataReader.ReadBytes(this->generalData.unknownPart1.unknown5, sizeof(this->generalData.unknownPart1.unknown5));
	dataReader.ReadBytes(this->generalData.unknownPart1.unknown6, sizeof(this->generalData.unknownPart1.unknown6));

	this->ReadAccompanimentSettings(dataReader);

	ASSERT(nKeyboardSettings <= 4, String::Number(nKeyboardSettings));
	for(uint8 i = 0; i < nKeyboardSettings; i++)
		this->ReadKeyboardSettings(this->keyboardSettings[i], dataReader);

	this->generalData.unknown99 = dataReader.ReadUInt32();
}

void PerformanceFormatV0_2Reader::Read0x02Chunk(uint8 versionMinor, DataReader &dataReader)
{
	auto& chunk = this->generalData._0x02000108_data;

	chunk.unknown01 = dataReader.ReadUInt32();
	chunk.unknown02 = dataReader.ReadUInt32();
	chunk.unknown03 = dataReader.ReadUInt32();
	chunk.unknown04 = dataReader.ReadUInt32();
	chunk.nKeyboardSettings = dataReader.ReadByte();
	chunk.unknown06 = dataReader.ReadByte();

	chunk.unknownPart11.unknown10 = dataReader.ReadByte();
	for(uint8 i = 0; i < 2; i++)
	{
		chunk.unknownPart11.unknownPart12[i].unknown11 = dataReader.ReadByte();
		chunk.unknownPart11.unknownPart12[i].unknown111 = dataReader.ReadByte();
		chunk.unknownPart11.unknownPart12[i].unknown112 = dataReader.ReadByte();
	}

	chunk.unknownPart11.unknown12 = dataReader.ReadInt32();


	chunk.unknownPart13.unknown20 = dataReader.ReadInt32();
	chunk.unknownPart13.unknown21 = dataReader.ReadInt32();
	chunk.unknownPart13.unknown22 = dataReader.ReadUInt32();
	chunk.unknownPart13.unknown23 = dataReader.ReadUInt32();
	chunk.unknownPart13.unknown24 = dataReader.ReadUInt32();
	chunk.unknownPart13.unknown25 = dataReader.ReadInt32();
	chunk.unknownPart13.unknown26 = dataReader.ReadInt32();
	chunk.unknownPart13.unknown27 = dataReader.ReadInt32();
	chunk.unknownPart13.unknown28 = dataReader.ReadInt32();

	for(uint8 i = 0; i < 8; i++)
	{
		auto& subChunk = chunk.unknownPart14[i];
		subChunk.unknown30 = dataReader.ReadUInt32();
		dataReader.ReadBytes(subChunk.unknown31, sizeof(subChunk.unknown31));
	}

	if(versionMinor == 1)
	{
		for (uint8 i = 0; i < 4; i++)
			this->ReadPadSettings(chunk.unknownPart15[i], dataReader);
	}

	ASSERT(chunk.nKeyboardSettings <= 4, String::Number(chunk.nKeyboardSettings));
	for(uint8 i = 0; i < chunk.nKeyboardSettings; i++)
	{
		auto& subChunk = chunk.unknownPart16[i];

		subChunk.unknownPart17.unknown50 = dataReader.ReadByte();
		dataReader.ReadBytes(subChunk.unknownPart17.unknown51, sizeof(subChunk.unknownPart17.unknown51));
		subChunk.unknownPart17.unknown52 = dataReader.ReadInt32();


		subChunk.unknownPart18.unknown53 = dataReader.ReadInt32();
		subChunk.unknownPart18.unknown54 = dataReader.ReadByte();
		subChunk.unknownPart18.unknown55 = dataReader.ReadUInt32();


		subChunk.unknownPart19.unknown56 = dataReader.ReadInt32();
		subChunk.unknownPart19.unknown57 = dataReader.ReadByte();
		subChunk.unknownPart19.unknown58 = dataReader.ReadByte();
		subChunk.unknownPart19.unknown59 = dataReader.ReadByte();
		subChunk.unknownPart19.unknown60 = dataReader.ReadByte();


		for(uint8 j = 0; j < 4; j++)
		{
			subChunk.unknownPart20[j].unknown70 = dataReader.ReadUInt16();
			dataReader.ReadBytes(subChunk.unknownPart20[j].unknown71, sizeof(subChunk.unknownPart20[j].unknown71));
		}

		for(uint8 j = 0; j < 4; j++)
		{
			subChunk.unknownPart21[j].unknown80 = dataReader.ReadUInt32();
			dataReader.ReadBytes(subChunk.unknownPart21[j].unknown81, sizeof(subChunk.unknownPart21[j].unknown81));
		}
	}

	chunk.unknownEnd = dataReader.ReadUInt32();
}

void PerformanceFormatV0_2Reader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x00000008:
			this->Read0x00000008Chunk(dataReader);
			break;
		case 0x02000008:
		case 0x02000108:
			this->Read0x02Chunk(chunkHeader.version.minor, dataReader);
			break;
	}
}

void PerformanceFormatV0_2Reader::ReadFX(FX& fx, DataReader& dataReader)
{
	dataReader.ReadBytes(fx.unknown9, 16);
}

//Private methods
void PerformanceFormatV0_2Reader::ReadAccompanimentSettings(DataReader &dataReader)
{
	this->generalData.unknown7 = dataReader.ReadInt8();

	this->ReadEffectGroup(this->generalData.unknown8, dataReader);

	for(uint8 i = 0; i < 8; i++)
	{
		this->ReadTrackParams(this->generalData.accompanimentSettings.trackSettings[i], dataReader);
	}
}

void PerformanceFormatV0_2Reader::ReadEffectGroup(FXGroup& fxGroup, DataReader &dataReader)
{
	for(uint8 i = 0; i < 2; i++)
	{
		fxGroup.fx[i].unknown8 = dataReader.ReadInt16();
		this->ReadFX(fxGroup.fx[i], dataReader);
	}

	fxGroup.unknown10 = dataReader.ReadInt16();
	fxGroup.unknown11 = dataReader.ReadByte();
	fxGroup.unknown12 = dataReader.ReadByte();
	fxGroup.unknown13 = dataReader.ReadByte();
	fxGroup.unknown14 = dataReader.ReadByte();
}

void PerformanceFormatV0_2Reader::ReadKeyboardSettings(KeyboardSettings& keyboardSettings, DataReader &dataReader)
{
	auto& sec = keyboardSettings;
	sec.unknown31 = dataReader.ReadByte();
	sec.unknown32 = dataReader.ReadByte();
	sec.unknown33 = dataReader.ReadByte();

	sec.unknownPart3.unknown34 = dataReader.ReadByte();
	sec.unknownPart3.unknown35 = dataReader.ReadByte();

	sec.unknownPart4.unknown36 = dataReader.ReadByte();
	sec.unknownPart4.unknown37 = dataReader.ReadByte();
	sec.unknownPart4.unknown38 = dataReader.ReadByte();
	sec.unknownPart4.unknown39 = dataReader.ReadInt8();
	sec.unknownPart4.unknown40 = dataReader.ReadInt8();
	sec.unknownPart4.unknown41 = dataReader.ReadByte();
	for(uint8 j = 0; j < 3; j++)
	{
		auto& chunk = sec.unknownPart4.unknownPart5[j];
		chunk.unknown42 = dataReader.ReadByte();
		chunk.unknown43 = dataReader.ReadByte();
	}

	for(uint8 j = 0; j < 4; j++)
		this->ReadPadSettings(sec.unknownPart4.padSettings[j], dataReader);

	ScaleSettingsReader scaleSettingsReader;
	scaleSettingsReader.Read(sec.scaleSettings, dataReader);

	this->ReadEffectGroup(sec.unknown8, dataReader);

	for(uint8 j = 0; j < 4; j++)
	{
		this->ReadTrackParams(sec.trackSettings[j], dataReader);
	}
}

void PerformanceFormatV0_2Reader::ReadPadSettings(PadSettings& padSettings, DataReader &dataReader)
{
	padSettings.unknown51 = dataReader.ReadUInt16();
	padSettings.unknown52 = dataReader.ReadByte();
	padSettings.unknown53 = dataReader.ReadByte();
	padSettings.unknown54 = dataReader.ReadInt8();
	padSettings.unknown55 = dataReader.ReadByte();
	padSettings.unknown56 = dataReader.ReadByte();
	padSettings.unknown57 = dataReader.ReadByte();
	padSettings.unknown58 = dataReader.ReadByte();
	padSettings.unknown59 = dataReader.ReadByte();
}

void PerformanceFormatV0_2Reader::ReadTrackParams(TrackSettings& unknownPart8, DataReader &dataReader)
{
	unknownPart8.unknown12 = dataReader.ReadUInt32();
	unknownPart8.unknown13 = dataReader.ReadByte();
	unknownPart8.unknown14 = dataReader.ReadByte();
	unknownPart8.unknown15 = dataReader.ReadByte();
	unknownPart8.unknown16 = dataReader.ReadByte();

	unknownPart8.soundProgramChangeSeq = ProgramChangeSequence::FromUInt32(dataReader.ReadUInt32());

	unknownPart8.unknown18 = dataReader.ReadByte();
	unknownPart8.unknown19 = dataReader.ReadInt8();
	unknownPart8.unknown20 = dataReader.ReadByte();
	unknownPart8.unknown21 = dataReader.ReadInt8();
	dataReader.ReadBytes(unknownPart8.unknown22, sizeof(unknownPart8.unknown22));
	unknownPart8.unknown23 = dataReader.ReadUInt32();
	unknownPart8.unknown24 = dataReader.ReadByte();
}
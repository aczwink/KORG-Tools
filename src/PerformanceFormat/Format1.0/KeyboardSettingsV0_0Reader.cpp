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
//Class header
#include "KeyboardSettingsV0_0Reader.hpp"
#include "TrackSettingsReader.hpp"
#include "PadSettingsReader.hpp"
#include "KeyboardSettingsDataReader.hpp"
#include "FXReader.hpp"
#include "../Format0.2/ScaleSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
libKORG::ChunkReader *KeyboardSettingsV0_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void KeyboardSettingsV0_0Reader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x07000008:
		{
			FXReader fxReader;
			fxReader.Read(this->keyboardSettings[this->perfIndex].fx, dataReader);
		}
		break;
		case 0x08000008:
		{
			PadSettingsReader padSettingsReader(this->keyboardSettings[this->perfIndex].padSettings);
			padSettingsReader.Read(dataReader);
		}
		break;
		case 0x09000008:
		{
			int32 trackNumber = dataReader.ReadInt32();
			TrackSettingsReader trackPropertiesReader;
			trackPropertiesReader.Read(this->keyboardSettings[this->perfIndex].trackSettings[trackNumber], dataReader);
		}
		break;
		case 0x0F000008:
		{
			auto& chunk = this->keyboardSettings[perfIndex];

			KeyboardSettingsDataReader keyboardSettingsDataReader;
			keyboardSettingsDataReader.Read0x0FChunk(chunk.name, chunk.unknown1, dataReader);
		}
		break;
		case 0x10000008:
		{
			KeyboardSettingsDataReader keyboardSettingsDataReader;
			keyboardSettingsDataReader.Read0x10000008Chunk(this->keyboardSettings[this->perfIndex]._0x10000008_data, dataReader);
		}
		break;
		case 0x11000008:
		{
			KeyboardSettingsDataReader keyboardSettingsDataReader;
			keyboardSettingsDataReader.Read0x11000008Chunk(this->keyboardSettings[this->perfIndex]._0x11000008_data, dataReader);
		}
		break;
		case 0x12000108:
		{
			KeyboardSettingsDataReader keyboardSettingsDataReader;
			keyboardSettingsDataReader.Read0x12000108Chunk(this->keyboardSettings[this->perfIndex]._0x12000108_data, dataReader);
		}
		break;
		case 0x13000008:
		{
			ScaleSettingsReader scaleSettingsReader;
			scaleSettingsReader.Read(this->keyboardSettings[this->perfIndex].scaleSettings, dataReader);
		}
		break;
		case 0x14000008:
		{
			KeyboardSettingsDataReader keyboardSettingsDataReader;
			keyboardSettingsDataReader.Read0x14000008Chunk(this->keyboardSettings[this->perfIndex]._0x14000008_data, dataReader);
		}
		break;
		case 0x15000008:
		case 0x15000108:
		{
			int32 trackIndex = dataReader.ReadInt32();
			this->keyboardSettings[this->perfIndex]._0x15000108_data[trackIndex].unknown = dataReader.ReadInt32();
		}
		break;
		case 0x18000008:
		{
			KeyboardSettingsDataReader keyboardSettingsDataReader;
			keyboardSettingsDataReader.Read0x18000008Chunk(this->keyboardSettings[this->perfIndex]._0x18000008_data, dataReader);
		}
		break;
	}
}

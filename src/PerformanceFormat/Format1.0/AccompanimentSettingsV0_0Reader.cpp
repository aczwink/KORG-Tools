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
#include "AccompanimentSettingsV0_0Reader.hpp"
//Local
#include "TrackSettingsReader.hpp"
#include "PadSettingsReader.hpp"
#include "FXReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

libKORG::ChunkReader *AccompanimentSettingsV0_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void AccompanimentSettingsV0_0Reader::ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x06000008:
		{
			auto& chunk = this->accompanimentSettings._0x06000008_data;
			chunk.unknown1 = dataReader.ReadInt8();
			chunk.unknown2 = dataReader.ReadInt8();
		}
		break;
		case 0x07000008:
		{
			FXReader fxReader;
			fxReader.Read(this->accompanimentSettings.fx, dataReader);
		}
		break;
		case 0x08000008:
		{
			PadSettingsReader padSettingsReader(this->accompanimentSettings.padSettings);
			padSettingsReader.Read(dataReader);
		}
		break;
		case 0x09000008:
		{
			int32 trackNumber = dataReader.ReadInt32();
			TrackSettingsReader trackPropertiesReader;
			trackPropertiesReader.Read(this->accompanimentSettings.trackSettings[trackNumber], dataReader);
		}
		break;
	}
}
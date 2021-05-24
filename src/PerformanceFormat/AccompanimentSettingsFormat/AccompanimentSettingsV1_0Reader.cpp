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
#include "AccompanimentSettingsV1_0Reader.hpp"
//Local
#include "../TrackPropertiesReader.hpp"
#include "../MasterFXReader.hpp"
#include "../PadSettingsReader.hpp"
#include "../InsertFXReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
libKORG::ChunkReader *AccompanimentSettingsV1_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	switch(chunkHeader.id)
	{
		case 0x1F000000:
			return nullptr;
	}
	return nullptr;
}

void AccompanimentSettingsV1_0Reader::ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x06000008:
		{
			this->accompanimentSettings._0x06000008_chunk.unknown1 = dataReader.ReadInt8();
			this->accompanimentSettings._0x06000008_chunk.unknown2 = dataReader.ReadInt8();
		}
		break;
		case 0x08000008:
		{
			PadSettingsReader padSettingsReader(this->accompanimentSettings.padSettings);
			padSettingsReader.Read(dataReader);
		}
		break;
		case 0x09010008:
		case 0x09010108:
		{
			TrackPropertiesReader trackPropertiesReader(this->accompanimentSettings.trackProperties);
			trackPropertiesReader.Read(chunkHeader, dataReader);

			this->max9version.major = Math::Max(this->max9version.major, chunkHeader.version.major);
			this->max9version.minor = Math::Max(this->max9version.minor, chunkHeader.version.minor);
		}
			break;
		case 0x1D000008:
		{
			MasterFXReader masterFxReader(this->accompanimentSettings.masterFxSettings);
			masterFxReader.Read(dataReader);
		}
		break;
		case 0x1E000008:
		{
			int32 trackIndex = dataReader.ReadInt32();
			int32 fxNumberOfTrack = dataReader.ReadInt32();

			InsertFXReader insertFxReader;
			insertFxReader.Read(this->accompanimentSettings.insertFXSettings[trackIndex][fxNumberOfTrack], dataReader);
		}
		break;
	}
}
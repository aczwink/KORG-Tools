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
#include "AccompanimentSettingsReader.hpp"
//Local
#include "../../Reading/TrackPropertiesReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
ChunkReader &AccompanimentSettingsReader::OnEnteringChunk(const ChunkHeader &chunkHeader)
{
	return *this;
}

void AccompanimentSettingsReader::ReadDataChunk(const ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch (chunkHeader.type)
	{
		case 6:
		case 7:
		case 8:
		case 29:
			this->accompanimentSettings.unknownChunksBefore9.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
			break;
		case 9:
		{
			TrackPropertiesReader trackPropertiesReader(this->accompanimentSettings.trackProperties);
			trackPropertiesReader.Read(chunkHeader, dataReader);

			this->max9version.major = Math::Max(this->max9version.major, chunkHeader.version.major);
			this->max9version.minor = Math::Max(this->max9version.minor, chunkHeader.version.minor);
		}
		break;
		case 30:
		case 31:
			this->accompanimentSettings.unknownChunksAfter9.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
		break;
	}
}

bool AccompanimentSettingsReader::IsDataChunk(const ChunkHeader &chunkHeader)
{
	if(chunkHeader.type == 31)
		return true;
	return ChunkReader::IsDataChunk(chunkHeader);
}
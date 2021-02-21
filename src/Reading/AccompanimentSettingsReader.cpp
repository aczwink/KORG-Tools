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
#include "TrackPropertiesReader.hpp"

//Protected methods
StdXX::String AccompanimentSettingsReader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/PERFORMANCE_ACCOMPANY/";
}

void AccompanimentSettingsReader::ReadDataChunk(const ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch (chunkHeader.type)
	{
		case 6:
			ASSERT_EQUALS(0, chunkHeader.version.major);
			ASSERT_EQUALS(0, chunkHeader.version.minor);
			ASSERT_EQUALS(0, dataReader.ReadUInt16());
			ASSERT_EQUALS(true, dataReader.InputStream().IsAtEnd());
			break;
		case 7:
		case 8:
		case 29:
			this->accompanimentSettings.unknownChunksBefore9.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
			break;
		case 9:
		{
			TrackPropertiesReader trackPropertiesReader(this->accompanimentSettings.trackProperties);
			trackPropertiesReader.Read(chunkHeader, dataReader);

			this->version.major = Math::Max(this->version.major, chunkHeader.version.major);
			this->version.minor = Math::Max(this->version.minor, chunkHeader.version.minor);
		}
		break;
		case 30:
			this->accompanimentSettings.unknownChunksAfter9.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
		break;
	}
}

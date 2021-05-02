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
#include "KeyboardSettingsReader.hpp"
#include "../../Reading/TrackPropertiesReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Protected methods
ChunkReader* KeyboardSettingsReader::OnEnteringChunk(const ChunkHeader &chunkHeader)
{
	return this;
}

void KeyboardSettingsReader::ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.type)
	{
		case 7:
		case 8:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 29:
			this->keyboardSettings[this->perfIndex].unknownChunksBeforeTracks.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
			break;
		case 9:
		{
			TrackPropertiesReader trackPropertiesReader(this->keyboardSettings[this->perfIndex].trackProperties);
			trackPropertiesReader.Read(chunkHeader, dataReader);
		}
		break;
		case 15:
			ASSERT_EQUALS(0, chunkHeader.version.major);
			ASSERT_EQUALS(0, chunkHeader.version.minor);
			this->Read0x0FChunk(dataReader);
			break;
		case 24:
		case 34:
			this->keyboardSettings[this->perfIndex].unknownChunksAfterTracks.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
			break;
	}
}

//Private methods
void KeyboardSettingsReader::Read0x0FChunk(DataReader &dataReader)
{
	uint32 perfIndex = dataReader.ReadUInt32();
	ASSERT(perfIndex <= 4, String::Number(perfIndex));
	perfIndex %= 4; //0-3 for style performances and 4 for performances

	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
	this->keyboardSettings[perfIndex].name = textReader.ReadZeroTerminatedString(37);
}

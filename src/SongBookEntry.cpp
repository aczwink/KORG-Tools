/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg/SongBookEntry.hpp>
//Namespaces
using namespace StdXX;
using namespace libKORG;

//Private methods
void SongBookEntry::ReadData(InputStream &inputStream)
{
	DataReader dataReader(true, inputStream);
	TextReader asciiReader(inputStream, TextCodecType::ASCII);
	TextReader utf16Reader(inputStream, TextCodecType::UTF16_BE);

	while(!inputStream.IsAtEnd())
	{
		uint32 chunkId = dataReader.ReadUInt32();
		uint32 chunkSize = dataReader.ReadUInt32();

		switch(chunkId)
		{
			case 0x208:
			{
				ASSERT_EQUALS(3, dataReader.ReadByte());
				String genre = asciiReader.ReadZeroTerminatedString();
				String artist = asciiReader.ReadZeroTerminatedString();
				uint8 tempo = dataReader.ReadByte();

				KeySignature key = (KeySignature)dataReader.ReadByte();
				Meter meter = (Meter)dataReader.ReadByte();

				uint32 unknown3 = dataReader.ReadUInt32();
				uint32 unknown4 = dataReader.ReadUInt32();

				stdOut << genre << endl;
				stdOut << artist << endl;

				genre = utf16Reader.ReadZeroTerminatedString();
				artist = utf16Reader.ReadZeroTerminatedString();

				stdOut << genre << endl;
				stdOut << artist << endl;
			}
			break;
			case 0x01000008:
			{
				ASSERT_EQUALS(1, chunkSize);
				ASSERT_EQUALS(0, dataReader.ReadByte());
			}
			break;
			case 0x02000008:
			{
				String textPath = asciiReader.ReadZeroTerminatedString(chunkSize);
				stdOut << textPath << endl;
			}
			break;
			case 0x03000008:
			{
				uint32 selectionNumber = dataReader.ReadUInt32();
				ThreeState memory = (ThreeState)dataReader.ReadByte();
				ThreeState synchroStart = (ThreeState)dataReader.ReadByte();
				ThreeState synchroStop = (ThreeState)dataReader.ReadByte();
			}
			break;
			case 0x04000008:
			{
				ASSERT_EQUALS(1, chunkSize);
				ASSERT_EQUALS(0, dataReader.ReadByte());
			}
			break;
			case 0x05000008:
			{
				ASSERT_EQUALS(2, chunkSize);
				ASSERT_EQUALS(0, dataReader.ReadUInt16());
			}
			break;
			case 0x06000008:
			{
				String textPath = utf16Reader.ReadZeroTerminatedString(chunkSize / 2);
				stdOut << textPath << endl;
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
}

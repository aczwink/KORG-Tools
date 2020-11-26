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
#include <libkorg/SongBook.hpp>
//Namespaces
using namespace StdXX;
using namespace libKORG;

//Private methods
void SongBook::ReadData(InputStream &inputStream)
{
	DataReader dataReader(true, inputStream);
	TextReader asciiReader(inputStream, TextCodecType::ASCII);
	TextReader utf16Reader(inputStream, TextCodecType::UTF16_BE);

	while (!inputStream.IsAtEnd())
	{
		uint32 unknown = dataReader.ReadUInt32();
		ASSERT_EQUALS(0x0108, unknown);
		uint32 nameSize = dataReader.ReadUInt32();

		String asciiName = asciiReader.ReadZeroTerminatedString();
		nameSize -= asciiName.GetSize();
		nameSize--; //zero byte

		String utf16name = utf16Reader.ReadZeroTerminatedString(nameSize / 2);
		stdOut << asciiName << " " << utf16name << endl;
	}
}

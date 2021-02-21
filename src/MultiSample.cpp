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
#include <libkorg/MultiSample.hpp>
//Namespaces
using namespace StdXX;
using namespace libKORG;

//Private methods
void MultiSample::ReadData(InputStream &inputStream)
{
	/*FileOutputStream fileOutputStream(FileSystem::Path(u8"/home/amir/Desktop/korg/_OUT/multisample"), true);
	inputStream.FlushTo(fileOutputStream);*/

	TextReader textReader(inputStream, TextCodecType::ASCII);
	DataReader dataReader(true, inputStream);

	uint16 version = dataReader.ReadUInt16();
	ASSERT((version == 2) || (version == 3), "???");

	uint16 nSampleEntries = dataReader.ReadUInt16();
	uint16 nOtherEntries = dataReader.ReadUInt16();
	uint16 nOtherOtherEntries = dataReader.ReadUInt16();
	uint16 nFourth = dataReader.ReadUInt16();

	for(uint32 i = 0; i < nSampleEntries; i++)
	{
		uint32 unknown3 = dataReader.ReadUInt32();
		uint8 unknown4 = dataReader.ReadByte();
		ASSERT((unknown4 == 0) || (unknown4 == 2) || (unknown4 == 1), "???" + String::Number(unknown4));

		uint8 unknown6 = dataReader.ReadByte(); //bits per sample?
		ASSERT((unknown6 == 16) || (unknown6 == 48), "???");

		uint8 unknown5 = dataReader.ReadByte();
		ASSERT((unknown5 == 0) || (unknown5 == 5) || (unknown5 == 0xFF), "???" + String::Number(unknown5));

		uint32 unknownRepetetive[9];
		for(uint8 j = 0; j < 9; j++)
			unknownRepetetive[j] = dataReader.ReadUInt32();

		ASSERT_EQUALS(0, dataReader.ReadByte());
		uint32 unknownRepetetive1 = dataReader.ReadUInt32();

		uint32 unknownRepetetive3 = dataReader.ReadUInt32();
		uint32 unknownRepetetive4 = dataReader.ReadUInt32();

		uint32 unknownRepetetive2 = dataReader.ReadUInt32();

		ASSERT_EQUALS(0, dataReader.ReadUInt64());

		String name = textReader.ReadZeroTerminatedString(16);
		stdOut << name << endl;
		uint64 unknown = dataReader.ReadUInt64(); //same as the first 8 bytes in the PCM file

		uint16 unknown9 = dataReader.ReadUInt16();
		uint32 sampleRate = dataReader.ReadUInt32();

		uint8 unknown2 = dataReader.ReadByte();
	}

	for(uint32 i = 0; i < nOtherEntries; i++)
	{
		uint8 unknown11 = dataReader.ReadByte();
		ASSERT( (unknown11 == 0)
			|| (unknown11 == 1)
			|| (unknown11 == 2)
			|| (unknown11 == 3)
			|| (unknown11 == 4)
			|| (unknown11 == 5)
			|| (unknown11 == 6)
			|| (unknown11 == 7)
			|| (unknown11 == 8)
			|| (unknown11 == 9), "???" + String::Number(unknown11));

		uint8 unknown2 = dataReader.ReadByte();
		uint8 unknown21 = dataReader.ReadByte();
		ASSERT((unknown21 == 1)
			|| (unknown21 == 2)
			|| (unknown21 == 3)
			|| (unknown21 == 4)
			|| (unknown21 == 5)
			|| (unknown21 == 6)
			|| (unknown21 == 0xff), "???" + String::Number(unknown21));
		uint8 unknown3 = dataReader.ReadByte();

		ASSERT_EQUALS(Unsigned<uint32>::Max(), dataReader.ReadUInt32());
		uint16 unknown31 = dataReader.ReadUInt16();
		ASSERT((unknown31 == 0x8100)
			|| (unknown31 == 0x8101)
			|| (unknown31 == 0x8102)
			|| (unknown31 == 0x8104)
			|| (unknown31 == 0x8106)
			|| (unknown31 == 0x8107)
			|| (unknown31 == 0x81FF), "???" + String::HexNumber(unknown31));
		uint16 unknown32 = dataReader.ReadUInt16();

		String name = textReader.ReadZeroTerminatedString(16);
		stdOut << i << " " << name << endl;

		uint32 unknown4 = dataReader.ReadUInt32();

		uint32 unknown41 = dataReader.ReadUInt32();
		ASSERT_EQUALS(0, dataReader.ReadUInt32());

		uint32 unknown5 = dataReader.ReadUInt32();
		uint16 unknown6 = dataReader.ReadUInt16();
		uint8 unknown7 = dataReader.ReadByte();

		uint8 unknown1 = dataReader.ReadByte();
	}

	for(uint32 i = 0; i < nOtherOtherEntries; i++)
	{
		uint16 unknown1 = dataReader.ReadUInt16();
		ASSERT_EQUALS(1, dataReader.ReadByte());

		String name = textReader.ReadString(2);
		stdOut << i << " " << name << endl;

		uint32 unknown3 = dataReader.ReadUInt32();

		uint8 unknown2 = dataReader.ReadByte();
		ASSERT((unknown2 == 0x40) || (unknown2 == 0), "???");

		ASSERT_EQUALS(0, dataReader.ReadUInt32());
		ASSERT_EQUALS(0, dataReader.ReadUInt32());
		ASSERT_EQUALS(0, dataReader.ReadByte());
		uint8 unknown4 = dataReader.ReadByte();
	}

	TextReader latin1TextReader(inputStream, TextCodecType::Latin1);
	for(uint32 i = 0; i < nFourth; i++)
	{
		ASSERT_EQUALS(0x80, dataReader.ReadByte());

		uint8 unknown11 = dataReader.ReadByte();
		ASSERT((unknown11 == 0) || (unknown11 == 1), "???");

		String name = latin1TextReader.ReadZeroTerminatedString(24);
		stdOut << i << " " << name << endl;

		uint16 unknown2 = dataReader.ReadUInt16();

		ASSERT_EQUALS(0, dataReader.ReadUInt16());

		dataReader.Skip(46);

		uint8 unknown3 = dataReader.ReadByte();
		ASSERT((unknown3 == 0)
			|| (unknown3 == 1)
			|| (unknown3 == 2)
			|| (unknown3 == 3)
			|| (unknown3 == 4)
			|| (unknown3 == 5)
			|| (unknown3 == 6)
			|| (unknown3 == 7)
			|| (unknown3 == 10)
			|| (unknown3 == 12)
			|| (unknown3 == 0xff), "???" + String::Number(unknown3));

		dataReader.Skip(0x58);
		uint8 unknown1 = dataReader.ReadByte();
	}
}
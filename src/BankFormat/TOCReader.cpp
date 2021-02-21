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
#include "TOCReader.hpp"
//Namespaces
using namespace BankFormat;

//Public methods
DynamicArray<HeaderEntry> TOCReader::Read()
{
	if(this->chunkHeader.version.major == 0)
	{
		ASSERT_EQUALS(0, this->chunkHeader.version.minor);
		this->ReadVersion0();
	}
	else if(this->chunkHeader.version.major == 1)
	{
		ASSERT((this->chunkHeader.version.minor == 0) || (this->chunkHeader.version.minor == 1), String::HexNumber(this->chunkHeader.version.minor));
		this->ReadVersion1(this->chunkHeader.version.minor);
	}
	else
		NOT_IMPLEMENTED_ERROR;

	return this->entries;
}

//Private methods
HeaderEntry TOCReader::ReadEntryVersion1_3()
{
	HeaderEntry headerEntry;

	this->ReadEntryVersion1_General(headerEntry);

	uint16 unknown1 = this->dataReader.ReadUInt16();
	ASSERT((unknown1 == 0)
		|| (unknown1 == 1)
		|| (unknown1 == 0x100)
		|| (unknown1 == 0x200), "???" + String::Number(unknown1));
	this->dataReader.ReadUInt32();

	return headerEntry;
}

HeaderEntry TOCReader::ReadEntryVersion1_4()
{
	HeaderEntry headerEntry;

	this->ReadEntryVersion1_General(headerEntry);

	ASSERT_EQUALS(5, this->dataReader.ReadUInt32());
	ASSERT_EQUALS(7, this->dataReader.ReadUInt16());
	ASSERT_EQUALS(0x14, this->dataReader.ReadByte());
	this->dataReader.ReadByte();
	this->dataReader.ReadUInt16();
	this->dataReader.ReadUInt16();
	this->dataReader.ReadUInt16();
	this->dataReader.ReadUInt16();
	this->dataReader.ReadByte();

	return headerEntry;
}

HeaderEntry TOCReader::ReadEntryVersion1_5()
{
	HeaderEntry headerEntry;

	this->ReadEntryVersion1_General(headerEntry);

	this->dataReader.ReadUInt16();
	ASSERT_EQUALS(5, this->dataReader.ReadUInt16());
	ASSERT_EQUALS(7, this->dataReader.ReadUInt16());
	ASSERT_EQUALS(0x14, this->dataReader.ReadByte());
	ASSERT_EQUALS(0x11, this->dataReader.ReadByte());

	this->dataReader.ReadByte();
	this->dataReader.ReadUInt32();
	ASSERT_EQUALS(6, this->dataReader.ReadUInt16());

	uint16 systemInfoSize = this->dataReader.ReadUInt16();
	textReader.ReadZeroTerminatedString(systemInfoSize);
	this->dataReader.ReadUInt32();

	return headerEntry;
}

void TOCReader::ReadEntryVersion1_General(HeaderEntry &headerEntry)
{
	ASSERT_EQUALS(2, this->dataReader.ReadUInt16());

	uint16 nameLength = this->dataReader.ReadUInt16();
	headerEntry.name = this->textReader.ReadString(nameLength);

	ASSERT_EQUALS(3_u32, this->dataReader.ReadUInt32());

	uint8 type = dataReader.ReadByte();
	ASSERT((type == (uint8)ObjectType::PCM)
		   || (type == (uint8)ObjectType::SongBookEntry)
		   || (type == (uint8)ObjectType::Style)
		   || (type == (uint8)ObjectType::StylePerformances)
	, "???");
	headerEntry.type = static_cast<ObjectType>(type);

	uint8 bankNumber = this->dataReader.ReadByte();
	headerEntry.pos = this->dataReader.ReadByte();

	ASSERT_EQUALS(1, this->dataReader.ReadUInt16());
	ASSERT_EQUALS(2, this->dataReader.ReadUInt16());
}

void TOCReader::ReadVersion0()
{
	uint32 nEntries = this->chunkHeader.size / BankFormat::OBJECTTOC_LINESIZE;
	this->entries.Resize(nEntries);

	for(uint32 i = 0; i < nEntries; i++)
	{
		BankFormat::HeaderEntry& headerEntry = this->entries[i];

		headerEntry.name = textReader.ReadZeroTerminatedString(BankFormat::HEADERENTRY_NAME_SIZE);

		uint8 type = dataReader.ReadByte();
		ASSERT(type == (uint8)ObjectType::MultiSample
			|| type == (uint8)ObjectType::PAD
			|| type == (uint8)ObjectType::PCM
			|| type == (uint8)ObjectType::Performance
			|| type == (uint8)ObjectType::SongBookEntry
			|| type == (uint8)ObjectType::SongBook
			|| type == (uint8)ObjectType::Sound
			|| type == (uint8)ObjectType::Style
			|| type == (uint8)ObjectType::StylePerformances, "???");
		headerEntry.type = static_cast<ObjectType>(type);

		uint8 bankNumber = dataReader.ReadByte();
		headerEntry.pos = dataReader.ReadByte();

		uint8 unknown = dataReader.ReadByte();
		ASSERT((unknown == 3)
		|| (unknown == 2)
		|| (unknown == 1)
		|| (unknown == 0), "???");

		uint16 unknown1 = dataReader.ReadUInt16();
		ASSERT((unknown1 == 0)
		|| (unknown1 == 0x200)
		|| (unknown1 == 0x300), "???");
	}
}

void TOCReader::ReadVersion1(uint8 versionMinor)
{
	uint32 leftTocSize = this->chunkHeader.size;
	while(leftTocSize)
	{
		HeaderEntry headerEntry;

		uint16 headerEntrySize = this->dataReader.ReadUInt16();
		leftTocSize -= 2;

		uint16 version = this->dataReader.ReadUInt16();
		switch(version)
		{
			case 3:
				headerEntry = this->ReadEntryVersion1_3();
				break;
			case 4:
				headerEntry = this->ReadEntryVersion1_4();
				break;
			case 5:
				headerEntry = this->ReadEntryVersion1_5();
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}

		leftTocSize -= headerEntrySize;
		this->entries.Push(headerEntry);
	}
}
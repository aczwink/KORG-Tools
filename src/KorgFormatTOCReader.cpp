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
#include "KorgFormatTOCReader.hpp"
//Namespaces
using namespace KorgFormat;

//Public methods
void KorgFormatTOCReader::Read()
{
	switch(this->tocId)
	{
		case (uint32)ChunkId::ObjectTOC:
			this->ReadStandard();
			break;
		case (uint32)ChunkId::ObjectTOC_Extended:
			this->ReadExtended();
			break;
		case (uint32)ChunkId::ObjectTOC_Extended2:
			this->ReadExtended2();
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

//Private methods
KorgFormat::HeaderEntry KorgFormatTOCReader::ReadEntryVersion3()
{
	KorgFormat::HeaderEntry headerEntry;

	ASSERT_EQUALS(2, this->dataReader.ReadUInt16());

	uint16 nameLength = this->dataReader.ReadUInt16();
	headerEntry.name = textReader.ReadString(nameLength);

	ASSERT_EQUALS(3_u32, this->dataReader.ReadUInt32());

	uint8 type = dataReader.ReadByte();
	ASSERT((type == (uint8)ObjectType::PCM)
		|| (type == (uint8)ObjectType::SongBookEntry)
		|| (type == (uint8)ObjectType::StylePerformances), "???");
	headerEntry.type = static_cast<ObjectType>(type);

	uint8 bankNumber = this->dataReader.ReadByte();
	headerEntry.pos = this->dataReader.ReadByte();

	ASSERT_EQUALS(1, this->dataReader.ReadUInt16());
	ASSERT_EQUALS(2, this->dataReader.ReadUInt16());
	uint16 unknown1 = this->dataReader.ReadUInt16();
	ASSERT((unknown1 == 0) || (unknown1 == 1) || (unknown1 == 0x100), "???" + String::Number(unknown1));
	this->dataReader.ReadUInt32();

	return headerEntry;
}

KorgFormat::HeaderEntry KorgFormatTOCReader::ReadEntryVersion5()
{
	KorgFormat::HeaderEntry headerEntry;

	ASSERT_EQUALS(0_u8, this->dataReader.ReadByte());
	ASSERT_EQUALS(0x0200_u16, this->dataReader.ReadUInt16());

	uint8 nameLength = this->dataReader.ReadByte();
	headerEntry.name = textReader.ReadString(nameLength);

	ASSERT_EQUALS(3_u32, this->dataReader.ReadUInt32());

	uint8 type = dataReader.ReadByte();
	ASSERT(type == (uint8)ObjectType::PCM, "???");
	headerEntry.type = static_cast<ObjectType>(type);

	uint8 bankNumber = dataReader.ReadByte();
	headerEntry.pos = this->dataReader.ReadByte();

	ASSERT_EQUALS(1_u16, this->dataReader.ReadUInt16());
	ASSERT_EQUALS(131072_u32, this->dataReader.ReadUInt32());
	ASSERT_EQUALS(196614, this->dataReader.ReadUInt32());
	ASSERT_EQUALS(16973826_u32, this->dataReader.ReadUInt32());
	ASSERT_EQUALS(2097156_u32, this->dataReader.ReadUInt32());

	ASSERT_EQUALS(0x0008, this->dataReader.ReadUInt16());

	uint8 unknown1 = this->dataReader.ReadByte();
	ASSERT((unknown1 == 1) || (unknown1 == 5) || (unknown1 == 6) || (unknown1 == 0x45), "???");

	uint8 unknown2 = this->dataReader.ReadByte();

	this->dataReader.ReadUInt32(); //???
	this->dataReader.ReadUInt32(); //???
	this->dataReader.ReadUInt16(); //???

	return headerEntry;
}

void KorgFormatTOCReader::ReadExtended()
{
	while(this->leftTocSize)
	{
		KorgFormat::HeaderEntry headerEntry;

		uint64 unknown = this->dataReader.ReadUInt64();
		ASSERT((unknown == 0x003300030002001C) || (unknown == 0x005200050002001C), "???");

		headerEntry.name = textReader.ReadZeroTerminatedString(31);

		ASSERT_EQUALS(3_u8, this->dataReader.ReadByte());

		uint8 type = dataReader.ReadByte();
		ASSERT(type == (uint8)ObjectType::Style || type == (uint8)ObjectType::StylePerformances, "???");
		headerEntry.type = static_cast<ObjectType>(type);

		ASSERT_EQUALS(101_u8, this->dataReader.ReadByte());
		headerEntry.pos = this->dataReader.ReadByte();
		ASSERT_EQUALS(0_u8, this->dataReader.ReadByte());
		ASSERT_EQUALS(0x0100_u16, this->dataReader.ReadUInt16());
		ASSERT_EQUALS(2_u8, this->dataReader.ReadByte());

		uint8 unknown2 = dataReader.ReadByte();
		if(headerEntry.type == ObjectType::Style)
		{
			ASSERT_EQUALS(0_u8, unknown2)
			ASSERT_EQUALS(0_u8, this->dataReader.ReadByte());
		}
		else
		{
			ASSERT_EQUALS(2_u8, unknown2)
			uint8 additionalIndicator = this->dataReader.ReadByte();

			ASSERT((additionalIndicator == 0) || (additionalIndicator == 1), "???");
			if(additionalIndicator)
			{
				ASSERT_EQUALS(0x000500071411071C, this->dataReader.ReadUInt64());
				ASSERT_EQUALS(0x09_u8, this->dataReader.ReadByte());
				this->dataReader.ReadUInt16();

				ASSERT_EQUALS(0x00060010_u32, this->dataReader.ReadUInt32());
				textReader.ReadZeroTerminatedString(16);

				this->leftTocSize -= 31;
			}
		}

		this->dataReader.ReadUInt32(); //??? maybe a checksum?

		this->leftTocSize -= 53;
		this->entries.Push(headerEntry);
	}
}

void KorgFormatTOCReader::ReadExtended2()
{
	while(this->leftTocSize)
	{
		KorgFormat::HeaderEntry headerEntry;

		uint16 headerEntrySize = this->dataReader.ReadUInt16();
		this->leftTocSize -= 2;

		uint16 version = this->dataReader.ReadUInt16();
		switch(version)
		{
			case 3:
				headerEntry = this->ReadEntryVersion3();
				break;
			case 5:
				headerEntry = this->ReadEntryVersion5();
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}

		this->leftTocSize -= headerEntrySize;
		this->entries.Push(headerEntry);
	}
}

void KorgFormatTOCReader::ReadStandard()
{
	uint32 nEntries = this->leftTocSize / KorgFormat::OBJECTTOC_LINESIZE;
	this->entries.Resize(nEntries);

	for(uint32 i = 0; i < nEntries; i++)
	{
		KorgFormat::HeaderEntry& headerEntry = this->entries[i];

		headerEntry.name = textReader.ReadZeroTerminatedString(KorgFormat::HEADERENTRY_NAME_SIZE);

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

		/*
		 * Standard banks: 0-
		 * User banks: 17, 18, 19
		 * Favorite Banks: 20-
		 */
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
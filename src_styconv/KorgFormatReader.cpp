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
#include "KorgFormatReader.hpp"
//Local
#include "Style.hpp"
#include "Performance.hpp"
//Namespaces
using namespace KorgFormat;

//Public methods
StyleBank KorgFormatReader::Read()
{
	this->ReadHeader();

	DynamicArray<KorgFormat::HeaderEntry> headerEntries = this->ReadTOC();

	StyleBank styleBank;
	this->ReadEntries(headerEntries, styleBank);

	ASSERT_EQUALS(0xFE000018, dataReader.ReadUInt32());
	uint32 kBegLength = dataReader.ReadUInt32();
	dataReader.Skip(kBegLength);

	stdOut << "Finished at: " << this->inputStream.GetCurrentOffset() << " - remaining: " << this->inputStream.QueryRemainingBytes() << endl;

	return styleBank;
}

//Private methods
ChunkHeader KorgFormatReader::ReadChunkHeader()
{
	ChunkHeader chunkHeader{};

	chunkHeader.id = this->dataReader.ReadUInt32();
	chunkHeader.size = this->dataReader.ReadUInt32();

	return chunkHeader;
}

void KorgFormatReader::ReadEntries(const DynamicArray<KorgFormat::HeaderEntry>& headerEntries, StyleBank& styleBank)
{
	for(const KorgFormat::HeaderEntry& headerEntry : headerEntries)
	{
		ChunkHeader chunkHeader = this->ReadChunkHeader();
		stdOut << String::HexNumber(chunkHeader.id) << " " << headerEntry.name << " " << (uint32)headerEntry.type << endl;

		ASSERT((chunkHeader.id == (uint32)ChunkId::StyleData)
		|| (chunkHeader.id == (uint32)ChunkId::PerformanceData)
		|| (chunkHeader.id == (uint32)ChunkId::PerformanceData_Extended)
		|| (chunkHeader.id == (uint32)ChunkId::PerformanceData_Extended2), "???");

		if(headerEntry.type == ObjectType::Style)
			styleBank.AddObject(headerEntry.pos, new Style(headerEntry.name, dataReader, chunkHeader.size));
		else
		{
			//ASSERT_EQUALS(0_u32, headerEntry.name.GetLength());
			styleBank.AddObject(headerEntry.pos, new Performance(dataReader, chunkHeader.size));
		}
	}
}

DynamicArray<KorgFormat::HeaderEntry> KorgFormatReader::ReadTOC()
{
	ChunkHeader tocHeader = this->ReadChunkHeader();

	if(tocHeader.id == (uint32)ChunkId::ObjectTOC_Extended)
		return this->ReadTOC_Extended(tocHeader.size);

	ASSERT_EQUALS((uint32)ChunkId::ObjectTOC, tocHeader.id);

	uint32 nEntries = tocHeader.size / KorgFormat::OBJECTTOC_LINESIZE;
	DynamicArray<KorgFormat::HeaderEntry> headerEntries;
	headerEntries.Resize(nEntries);

	TextReader textReader(this->inputStream, TextCodecType::ASCII);
	for(uint32 i = 0; i < nEntries; i++)
	{
		KorgFormat::HeaderEntry& headerEntry = headerEntries[i];

		headerEntry.name = textReader.ReadZeroTerminatedString(KorgFormat::HEADERENTRY_NAME_SIZE);

		uint8 type = dataReader.ReadByte();
		ASSERT(type == (uint8)ObjectType::Style || type == (uint8)ObjectType::Performance, "???");
		headerEntry.type = static_cast<ObjectType>(type);

		/*
		 * Standard banks: 0-
		 * User banks: 17, 18, 19
		 * Favorite Banks: 20-
		 */
		uint8 bankNumber = dataReader.ReadByte();

		headerEntry.pos = dataReader.ReadByte();

		uint8 unknown = dataReader.ReadByte();
		if(headerEntry.type == ObjectType::Style)
			ASSERT_EQUALS(0_u8, unknown)
		else
		{
			//600qt: 1,
			//4xor: 2
			ASSERT((unknown == 2) || (unknown == 1), "???")
		}

		ASSERT_EQUALS(0_u16, dataReader.ReadUInt16());
	}

	return headerEntries;
}

DynamicArray<KorgFormat::HeaderEntry> KorgFormatReader::ReadTOC_Extended(uint32 tocSize)
{
	DynamicArray<KorgFormat::HeaderEntry> headerEntries;

	TextReader textReader(this->inputStream, TextCodecType::ASCII);
	while(tocSize)
	{
		KorgFormat::HeaderEntry headerEntry;

		uint64 unknown = this->dataReader.ReadUInt64();
		ASSERT((unknown == 0x003300030002001C) || (unknown == 0x005200050002001C), "???");

		headerEntry.name = textReader.ReadZeroTerminatedString(31);

		ASSERT_EQUALS(3_u8, this->dataReader.ReadByte());

		uint8 type = dataReader.ReadByte();
		ASSERT(type == (uint8)ObjectType::Style || type == (uint8)ObjectType::Performance, "???");
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

				tocSize -= 31;
			}
		}

		this->dataReader.ReadUInt32(); //??? maybe a checksum?

		tocSize -= 53;
		headerEntries.Push(headerEntry);
	}

	return headerEntries;
}

void KorgFormatReader::ReadHeader()
{
	DataReader fourccReader(false, inputStream);

	ChunkHeader mainHeader = this->ReadChunkHeader();
	ASSERT_EQUALS((uint32)ChunkId::Container, mainHeader.id);
	ASSERT_EQUALS(inputStream.QuerySize() - 8, (uint64)mainHeader.size);

	//korf chunk
	ChunkHeader korfHeader = this->ReadChunkHeader();
	ASSERT_EQUALS((uint32)ChunkId::KorgFile, korfHeader.id);
	ASSERT_EQUALS(12_u32, korfHeader.size);

	ASSERT_EQUALS(0x0B000000_u32, dataReader.ReadUInt32());
	ASSERT_EQUALS(0_u16, dataReader.ReadUInt16());
	ASSERT_EQUALS(0_u8, dataReader.ReadByte());
	ASSERT_EQUALS(FOURCC(u8"KORF"), fourccReader.ReadUInt32());
	ASSERT_EQUALS(0_u8, dataReader.ReadByte());
}
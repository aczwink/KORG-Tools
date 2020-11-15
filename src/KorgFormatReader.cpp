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
#include "../include/libkorg/KorgFormatReader.hpp"
//Local
#include <libkorg/Sample.hpp>
#include "../include/libkorg/Style.hpp"
#include "../include/libkorg/Performance.hpp"
#include "OC31Decompressor.hpp"
#include "KorgFormatTOCReader.hpp"
//Namespaces
using namespace KorgFormat;
using namespace libKORG;

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
		stdOut << String::HexNumber(chunkHeader.id) << " " << headerEntry.name << " " << (uint32)headerEntry.type << " " << this->inputStream.GetCurrentOffset() << endl;

		ASSERT((chunkHeader.id == (uint32)ChunkId::StyleData)
		|| (chunkHeader.id == (uint32)ChunkId::PerformanceData)
		|| (chunkHeader.id == (uint32)ChunkId::PerformanceData_Extended)
		|| (chunkHeader.id == (uint32)ChunkId::PerformanceData_Extended2)
		|| (chunkHeader.id == (uint32)ChunkId::PCMData1)
		|| (chunkHeader.id == (uint32)ChunkId::PCMData2), "???");

		uint32 oc31Header = this->CreateFourCCReader().ReadUInt32();
		ASSERT_EQUALS(oc31Header, FOURCC(u8"OC31"));
		uint32 oc31BlockSize = chunkHeader.size - 4; //subtract "OC31" header
		LimitedInputStream limitedInputStream(this->inputStream, oc31BlockSize);
		BufferedInputStream bufferedInputStream(limitedInputStream);
		OC31Decompressor decompressor(bufferedInputStream);

		StyleBankObject* object;
		switch(headerEntry.type)
		{
			case ObjectType::Performance:
				object = new Performance(decompressor);
				break;
			case ObjectType::Style:
				object = new Style(headerEntry.name, decompressor);
				break;
			case ObjectType::PCM:
				object = new Sample(decompressor);
				break;
			case ObjectType::StylePerformances:
				object = new Performance(decompressor);
				break;
		}

		styleBank.AddObject(headerEntry.pos, object);
	}
}

DynamicArray<KorgFormat::HeaderEntry> KorgFormatReader::ReadTOC()
{
	ChunkHeader tocHeader = this->ReadChunkHeader();
	KorgFormatTOCReader tocReader(tocHeader.id, tocHeader.size, this->inputStream, this->dataReader);

	tocReader.Read();
	return tocReader.Entries();
}

void KorgFormatReader::ReadHeader()
{
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
	ASSERT_EQUALS(FOURCC(u8"KORF"), this->CreateFourCCReader().ReadUInt32());
	ASSERT_EQUALS(0_u8, dataReader.ReadByte());
}
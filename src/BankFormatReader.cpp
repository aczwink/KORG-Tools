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
#include "../include/libkorg/BankFormatReader.hpp"
//Local
#include <libkorg/Sample.hpp>
#include <libkorg/EncryptedSample.hpp>
#include <libkorg/MultiSample.hpp>
#include <libkorg/SongBook.hpp>
#include <libkorg/SongBookEntry.hpp>
#include <libkorg/Sound.hpp>
#include <libkorg/Pad.hpp>
#include "../include/libkorg/Style.hpp"
#include "../include/libkorg/Performance.hpp"
#include "OC31Decompressor.hpp"
#include "KorgFormatTOCReader.hpp"
#include "Reading/PerformanceReader.hpp"
#include "Reading/StyleReader.hpp"
//Namespaces
using namespace KorgFormat;
using namespace libKORG;

//Public methods
DynamicArray<BankObjectEntry> BankFormatReader::Read()
{
	this->ReadHeader();

	DynamicArray<KorgFormat::HeaderEntry> headerEntries = this->ReadTOC();
	this->ReadEntries(headerEntries);

	ASSERT_EQUALS(0xFE000018, dataReader.ReadUInt32());
	uint32 kBegLength = dataReader.ReadUInt32();
	dataReader.Skip(kBegLength);

	ASSERT_EQUALS(0, this->inputStream.QueryRemainingBytes());

	return this->objectEntries;
}

//Private methods
ChunkHeader BankFormatReader::ReadChunkHeader()
{
	ChunkHeader chunkHeader{};

	uint32 tmp = this->dataReader.ReadUInt32();

	chunkHeader.id = tmp >> 8u;
	chunkHeader.flags = tmp & 0xFFu;
	chunkHeader.size = this->dataReader.ReadUInt32();

	ASSERT(chunkHeader.flags & (uint8)ChunkHeaderFlags::AlwaysSet, u8"AlwaysSet is always set");

	uint8 flagsCopy = chunkHeader.flags;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::AlwaysSet;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::Unknown2;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::Unknown3;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::OC31Compressed;
	flagsCopy &= ~(uint8)ChunkHeaderFlags::Encrypted;

	ASSERT_EQUALS(0, flagsCopy);

	return chunkHeader;
}

void BankFormatReader::ReadEntries(const DynamicArray<KorgFormat::HeaderEntry>& headerEntries)
{
	for(const KorgFormat::HeaderEntry& headerEntry : headerEntries)
	{
		ChunkHeader chunkHeader = this->ReadChunkHeader();
		/*stdOut << String::HexNumber(chunkHeader.id) << " " << headerEntry.name << " " << (uint32)headerEntry.type
			<< " " << headerEntry.pos << " " << this->inputStream.GetCurrentOffset() << endl;*/

		ASSERT((chunkHeader.id == (uint32)ChunkId::MultiSampleData)
			|| (chunkHeader.id == (uint32)ChunkId::OldSoundDataMaybe)
			|| (chunkHeader.id == (uint32)ChunkId::PadData)
			|| (chunkHeader.id == (uint32)ChunkId::PCMData)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData02)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData03)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData10)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData20)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData21)
			|| (chunkHeader.id == (uint32)ChunkId::SongBookListData)
			|| (chunkHeader.id == (uint32)ChunkId::SongBookListData1)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData00)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData0)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData1)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData2)
			|| (chunkHeader.id == (uint32)ChunkId::StyleData), "???" + String::HexNumber(chunkHeader.id));

		ChainedInputStream chainedInputStream(new LimitedInputStream(this->inputStream, chunkHeader.size));
		chainedInputStream.Add(new BufferedInputStream(chainedInputStream.GetEnd()));

		if(chunkHeader.flags & (uint8)ChunkHeaderFlags::Encrypted)
		{
			ASSERT_EQUALS(ObjectType::PCM, headerEntry.type);
		}
		else if(chunkHeader.flags & (uint8)ChunkHeaderFlags::OC31Compressed)
		{
			uint32 oc31Header = this->CreateFourCCReader(chainedInputStream.GetEnd()).ReadUInt32();
			ASSERT( (oc31Header == FOURCC(u8"OC31")) || (oc31Header == FOURCC(u8"OC32")), "???" + String::HexNumber(oc31Header));
			chainedInputStream.Add(new OC31Decompressor(chainedInputStream.GetEnd()));
		}

		InputStream& inputStream = chainedInputStream.GetEnd();
		BankObject* object;
		switch(headerEntry.type)
		{
			case ObjectType::MultiSample:
				object = new MultiSample(inputStream);
				break;
			case ObjectType::Performance:
			{
				PerformanceReader performanceReader;
				performanceReader.ReadData(inputStream);
				object = performanceReader.TakePerformanceResult();
			}
			break;
			case ObjectType::Style:
			{
				StyleReader styleReader;
				styleReader.ReadData(inputStream);
				object = styleReader.TakeResult();
			}
			break;
			case ObjectType::PAD:
				NOT_IMPLEMENTED_ERROR;
				//object = this->ReadInObect(new Pad, inputStream);
				break;
			case ObjectType::PCM:
				if(chunkHeader.flags & (uint8)ChunkHeaderFlags::Encrypted)
					object = new EncryptedSample(inputStream);
				else
					object = new Sample(inputStream);
				break;
			case ObjectType::SongBookEntry:
				object = new SongBookEntry(inputStream);
				break;
			case ObjectType::SongBook:
				object = new SongBook(inputStream);
				break;
			case ObjectType::Sound:
				object = new Sound(inputStream);
				break;
			case ObjectType::StylePerformances:
			{
				PerformanceReader performanceReader;
				performanceReader.ReadData(inputStream);
				object = performanceReader.TakeSTSResult();
			}
			break;
		}

		this->objectEntries.Push({ headerEntry.name, headerEntry.pos, object });

		//make sure we read all of it
		while(!inputStream.IsAtEnd())
		{
			uint32 nBytesSkipped = inputStream.Skip(c_io_blockSize);
			stdErr << nBytesSkipped << u8" bytes of trailing data found..." << endl;
		}
	}
}

DynamicArray<KorgFormat::HeaderEntry> BankFormatReader::ReadTOC()
{
	ChunkHeader tocHeader = this->ReadChunkHeader();
	KorgFormatTOCReader tocReader(tocHeader.id, tocHeader.size, this->inputStream, this->dataReader);

	tocReader.Read();
	return tocReader.Entries();
}

void BankFormatReader::ReadHeader()
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
	ASSERT_EQUALS(FOURCC(u8"KORF"), this->CreateFourCCReader(this->inputStream).ReadUInt32());
	ASSERT_EQUALS(0_u8, dataReader.ReadByte());
}
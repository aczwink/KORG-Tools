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
#include <libkorg/BankFormat/Reader.hpp>
//Local
#include <libkorg/BankFormat/SampleObject.hpp>
#include <libkorg/BankFormat/EncryptedSample.hpp>
#include "OC31Decompressor.hpp"
#include "TOCReader.hpp"
#include "../StyleFormat/StyleReader.hpp"
#include "../PCMFormat/PCMReader.hpp"
#include "../SoundFormat/SoundReader.hpp"
#include "../PerformanceFormat/PerformanceReaderZeroChunkReader.hpp"
#include "../MultiSamplesFormat/MultiSamplesReader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::BankFormat;
using namespace StdXX;

//Public methods
BankObject *Reader::ReadBankObject(const HeaderEntry &headerEntry, InputStream &inputStream)
{
	ChunkHeader chunkHeader;
	auto chunkInputStream = ChunkReader::ReadNextChunk(inputStream, chunkHeader);

	switch(ChunkType(chunkHeader.type))
	{
		case ChunkType::MultiSampleData:
		case ChunkType::LegacySoundData:
		case ChunkType::PCMData:
		case ChunkType::SoundData:
		{
			this->VerifyDataType(headerEntry, chunkHeader.type);
			this->VerifyDataVersion(headerEntry, chunkHeader.version);

			DataReader dataReader(true, *chunkInputStream);
			return this->ReadBankObject((ChunkType)chunkHeader.type, chunkHeader, headerEntry, dataReader);
		}
		case ChunkType::PadData:
		case ChunkType::PerformancesData:
		case ChunkType::StyleData:
		{
			this->VerifyData(headerEntry, chunkHeader);

			auto reader = this->OnEnteringChunkedResourceChunk(chunkHeader, headerEntry);
			reader->ReadData(*chunkInputStream);

			BankObject* bankObject = this->objectReader->TakeResult();
			return bankObject; //stdErr << u8"Can't read object: " << headerEntry.name << u8" with version: " << headerEntry.dataVersion.major << u8"." << headerEntry.dataVersion.minor << endl;
		}
	}
}

void Reader::ReadMetadata(SeekableInputStream& seekableInputStream)
{
	seekableInputStream.SeekTo(0);
	this->ReadContainerChunkHeader(seekableInputStream);
	this->ReadKorfHeaderChunk(seekableInputStream);
	this->ReadTableOfContentsChunk(seekableInputStream);
	this->FindXRefLocation(seekableInputStream);
	this->ReadXRef(seekableInputStream);
}

//Protected methods
BankObject* Reader::ReadBankObject(ChunkType chunkType, const ChunkHeader &chunkHeader, const HeaderEntry &headerEntry, DataReader &dataReader)
{
	BankObject* object = nullptr;

	switch(chunkType)
	{
		case ChunkType::MultiSampleData:
		{
			MultiSamplesReader multiSamplesReader;
			object = multiSamplesReader.Read(chunkHeader.version, dataReader);
		}
		break;
		case ChunkType::LegacySoundData:
		{
			SoundReader soundReader;
			object = soundReader.ReadLegacyData(chunkHeader.version, dataReader);
		}
		break;
		case ChunkType::PCMData:
		{
			if(chunkHeader.flags & (uint8)ChunkHeaderFlags::Encrypted)
			{
				bool isCompressed = chunkHeader.flags & (uint8)ChunkHeaderFlags::OC31Compressed;
				object = new EncryptedSample(*headerEntry.id, headerEntry.dataVersion, isCompressed, headerEntry.encryptionInformation, dataReader.InputStream());
			}
			else
			{
				PCMReader pcmReader;
				object = pcmReader.Read(chunkHeader.version, dataReader);
			}
		}
		break;
		case ChunkType::SoundData:
		{
			SoundReader soundReader;
			object = soundReader.ReadData(chunkHeader.version, dataReader);
		}
		break;
	}

	return object;
}

ChunkReader* Reader::OnEnteringChunkedResourceChunk(const ChunkHeader &chunkHeader, const HeaderEntry& headerEntry)
{
	switch(ChunkType(chunkHeader.type))
	{
		case ChunkType::PerformancesData:
		{
			this->objectReader = PerformanceReaderZeroChunkReader::CreateInstance(headerEntry.type == ObjectType::StylePerformances, chunkHeader.version);
			return this->objectReader.IsNull() ? nullptr : this->objectReader.operator->();
		}
		case ChunkType::PadData:
		case ChunkType::StyleData:
		{
			this->objectReader = StyleReader::CreateInstance(chunkHeader.version);
			return this->objectReader.operator->();
		}
	}

	return nullptr;
}

//Private methods
void Reader::FindXRefLocation(SeekableInputStream& seekableInputStream)
{
	seekableInputStream.SeekTo(seekableInputStream.QuerySize() - 4);
	DataReader dataReader(true, seekableInputStream);

	uint32 nEntries = dataReader.ReadUInt32();

	uint32 offset = nEntries * 4;
	offset += 8; //KBEG and KEND
	offset += 8; //chunk header
	offset += 4; //number of entries marker

	seekableInputStream.SeekTo(seekableInputStream.QuerySize() - offset);
}

void Reader::ReadContainerChunkHeader(InputStream& inputStream)
{
	ChunkHeader chunkHeader;
	ChunkReader::ReadNextChunk(inputStream, chunkHeader);

	ASSERT_EQUALS((uint8)ChunkType::Container, chunkHeader.type);
	ASSERT_EQUALS((uint8)ChunkHeaderFlags::Unknown4, chunkHeader.flags & (uint8)ChunkHeaderFlags::Unknown4);
	ASSERT_EQUALS((uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile, chunkHeader.flags & (uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
}

void Reader::ReadKorfHeaderChunk(InputStream &inputStream)
{
	ChunkHeader chunkHeader;
	auto chunkInputStream = ChunkReader::ReadNextChunk(inputStream, chunkHeader);

	ASSERT_EQUALS((uint8)ChunkType::KorgFile, chunkHeader.type);
	ASSERT_EQUALS((uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile, chunkHeader.flags & (uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile);

	ASSERT_EQUALS(12_u32, chunkHeader.size);

	DataReader dataReader(true, *chunkInputStream);

	ASSERT_EQUALS(0x0B000000_u32, dataReader.ReadUInt32());
	ASSERT_EQUALS(0_u16, dataReader.ReadUInt16());
	ASSERT_EQUALS(0_u8, dataReader.ReadByte());
	ASSERT_EQUALS(FOURCC(u8"KORF"), ChunkReader::ReadFourCC(dataReader.InputStream()));
	ASSERT_EQUALS(0_u8, dataReader.ReadByte());
}

void Reader::ReadTableOfContentsChunk(InputStream &inputStream)
{
	ChunkHeader chunkHeader;
	auto chunkInputStream = ChunkReader::ReadNextChunk(inputStream, chunkHeader);

	ASSERT_EQUALS((uint8)ChunkType::ObjectTOC, chunkHeader.type);
	ASSERT_EQUALS((uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile, chunkHeader.flags & (uint8)ChunkHeaderFlags::UnknownAlwaysSetInBankFile);

	TOCReader tocReader(chunkHeader, *chunkInputStream);
	this->headerEntries = tocReader.Read();
}

void Reader::ReadXRef(InputStream &inputStream)
{
	ChunkHeader chunkHeader;
	auto chunkInputStream = ChunkReader::ReadNextChunk(inputStream, chunkHeader);
	DataReader dataReader(true, *chunkInputStream);

	ASSERT_EQUALS(FOURCC(u8"KBEG"), ChunkReader::ReadFourCC(dataReader.InputStream()));
	for(uint32 leftSize = chunkHeader.size - 12; leftSize; leftSize -= 4)
		this->xref.Push(dataReader.ReadUInt32());
}
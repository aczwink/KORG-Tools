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
#include "../StyleFormat/Format0.0/StyleFormat0_0V5_0Reader.hpp"
#include "../StyleFormat/StyleReader.hpp"
#include "../PCMFormat/PCMReader.hpp"
#include "../SoundFormat/SoundReader.hpp"
#include "../PerformanceFormat/PerformanceReaderZeroChunkReader.hpp"
#include "../MultiSamplesFormat/MultiSamplesReader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::BankFormat;
using namespace StdXX;

void Reader::ReadBankObject(ChunkType chunkType, const ChunkHeader &chunkHeader, const HeaderEntry &headerEntry, DataReader &dataReader)
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

	if(object)
		this->AddObject(object, headerEntry);
}

ChunkReader* Reader::OnEnteringChunk(const ChunkHeader &chunkHeader)
{
	switch(ChunkType(chunkHeader.type))
	{
		case ChunkType::Container:
			return this;
		case ChunkType::PadData:
		case ChunkType::PerformancesData:
		case ChunkType::StyleData:
		{
			this->VerifyData(chunkHeader);

			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex];
			return this->OnEnteringChunkedResourceChunk(chunkHeader, headerEntry);
		}
	}

	return nullptr;
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

void Reader::OnLeavingChunk(const ChunkHeader &chunkHeader)
{
	switch(ChunkType(chunkHeader.type))
	{
		case ChunkType::PadData:
		case ChunkType::PerformancesData:
		case ChunkType::StyleData:
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex];
			if(!this->objectReader.IsNull())
			{
				BankObject* bankObject = this->objectReader->TakeResult();
				if(bankObject)
					this->AddObject(bankObject, headerEntry);
				else
					stdErr << u8"Can't read object: " << headerEntry.name << u8" with version: " << headerEntry.dataVersion.major << u8"." << headerEntry.dataVersion.minor << endl;
			}
			this->currentHeaderEntryIndex++;
		}
		break;
	}
}

void Reader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	ChunkType chunkType = (ChunkType)chunkHeader.type;
	switch(chunkType)
	{
		case ChunkType::CrossReferenceTable:
			dataReader.Skip(chunkHeader.size);
			break;
		case ChunkType::KorgFile:
		{
			ASSERT_EQUALS(12_u32, chunkHeader.size);

			ASSERT_EQUALS(0x0B000000_u32, dataReader.ReadUInt32());
			ASSERT_EQUALS(0_u16, dataReader.ReadUInt16());
			ASSERT_EQUALS(0_u8, dataReader.ReadByte());
			ASSERT_EQUALS(FOURCC(u8"KORF"), this->ReadFourCC(dataReader.InputStream()));
			ASSERT_EQUALS(0_u8, dataReader.ReadByte());
		}
		break;
		case ChunkType::ObjectTOC:
		{
			TOCReader tocReader(chunkHeader, dataReader.InputStream());
			this->headerEntries = tocReader.Read();
		}
		break;
		case ChunkType::MultiSampleData:
		case ChunkType::LegacySoundData:
		case ChunkType::PCMData:
		case ChunkType::SoundData:
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex];
			this->VerifyDataType(headerEntry, chunkHeader.type);
			this->VerifyDataVersion(headerEntry, chunkHeader.version);

			this->ReadBankObject(chunkType, chunkHeader, headerEntry, dataReader);
			this->currentHeaderEntryIndex++;
		}
		break;
		default:
			stdOut << String::HexNumber(chunkHeader.id) << endl;
			NOT_IMPLEMENTED_ERROR;
	}
}
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
#include "TOCEntryChecksumFunction.hpp"
//Namespaces
using namespace BankFormat;
using namespace StdXX;

//Public methods
DynamicArray<HeaderEntry> TOCReader::Read()
{
	if(this->chunkHeader.version.major == 0)
	{
		ASSERT_EQUALS(0, this->chunkHeader.version.minor);
		auto dataReader = this->CreateDataReader(this->inputStream);
		TextReader textReader(this->inputStream, TextCodecType::ASCII);
		this->ReadVersion0(dataReader, textReader);
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
ObjectType TOCReader::ReadObjectType(DataReader& dataReader)
{
	uint8 type = dataReader.ReadByte();
	ASSERT(type == (uint8)ObjectType::MultiSample
		   || type == (uint8)ObjectType::PAD
		   || type == (uint8)ObjectType::PCM
		   || type == (uint8)ObjectType::Performance
		   || type == (uint8)ObjectType::SongBookEntry
		   || type == (uint8)ObjectType::SongBook
		   || type == (uint8)ObjectType::Sound
		   || type == (uint8)ObjectType::Style
		   || type == (uint8)ObjectType::StylePerformances, String::Number(type));

	return static_cast<ObjectType>(type);
}

void TOCReader::ReadProperty(uint16 propertyType, uint16 propertySize, HeaderEntry& headerEntry, DataReader& dataReader, TextReader& textReader)
{
	switch(propertyType)
	{
		case 0:
		{
			ASSERT_EQUALS(3, propertySize);

			headerEntry.type = this->ReadObjectType(dataReader);
			uint8 bankNumber = dataReader.ReadByte();
			headerEntry.pos = dataReader.ReadByte();
		}
		break;
		case 1:
		{
			ASSERT_EQUALS(2, propertySize);

			headerEntry.dataVersion.major = dataReader.ReadByte();
			headerEntry.dataVersion.minor = dataReader.ReadByte();
		}
		break;
		case 2:
			headerEntry.name = textReader.ReadZeroTerminatedStringBySize(propertySize);
			break;
		case 3:
		{
			ASSERT_EQUALS(6, propertySize);

			auto& encInfo = headerEntry.encryptionInformation;

			encInfo.serialNumberType = static_cast<SerialNumberType>(dataReader.ReadByte());
			ASSERT(encInfo.serialNumberType == BankFormat::SerialNumberType::Machine
					|| encInfo.serialNumberType == BankFormat::SerialNumberType::FMDriver, String::Number((uint8)encInfo.serialNumberType));

			encInfo.encryptionAlgorithm = static_cast<EncryptionAlgorithm>(dataReader.ReadByte());
			ASSERT(encInfo.encryptionAlgorithm == BankFormat::EncryptionAlgorithm::Normal
				   || encInfo.encryptionAlgorithm == BankFormat::EncryptionAlgorithm::DES
				   || encInfo.encryptionAlgorithm == BankFormat::EncryptionAlgorithm::Mixed
				   || encInfo.encryptionAlgorithm == BankFormat::EncryptionAlgorithm::Blowfish
				   , String::Number((uint8)encInfo.encryptionAlgorithm));

			headerEntry.encryptionInformation.vendorId = dataReader.ReadUInt16();
			headerEntry.encryptionInformation.featureId = dataReader.ReadUInt16();
		}
		break;
		case 4:
		{
			ASSERT_EQUALS(8, propertySize);
			headerEntry.id = dataReader.ReadUInt64();
		}
		break;
		case 5: //unknown
		{
			ASSERT_EQUALS(7, propertySize);

			//4 bytes date
			ASSERT_EQUALS(0x14, dataReader.ReadByte());
			uint8 unknown2 = dataReader.ReadByte();
			uint8 unknown3 = dataReader.ReadByte();
			uint8 unknown4 = dataReader.ReadByte();
			//3 bytes time
			uint8 unknown5 = dataReader.ReadByte();
			uint8 unknown6 = dataReader.ReadByte();
			uint8 unknown7 = dataReader.ReadByte();
		}
			break;
		case 6: //system information
			textReader.ReadZeroTerminatedString(propertySize);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

void TOCReader::ReadVersion0(DataReader& dataReader, TextReader& textReader)
{
	uint32 nEntries = this->chunkHeader.size / BankFormat::OBJECTTOC_LINESIZE;
	this->entries.Resize(nEntries);

	for(uint32 i = 0; i < nEntries; i++)
	{
		BankFormat::HeaderEntry& headerEntry = this->entries[i];

		headerEntry.name = textReader.ReadZeroTerminatedString(BankFormat::HEADERENTRY_NAME_SIZE);
		headerEntry.type = this->ReadObjectType(dataReader);

		uint8 bankNumber = dataReader.ReadByte();
		headerEntry.pos = dataReader.ReadByte();

		headerEntry.dataVersion.major = dataReader.ReadByte();
		headerEntry.dataVersion.minor = dataReader.ReadByte();

		ASSERT_EQUALS(0, dataReader.ReadByte());
	}
}

void TOCReader::ReadVersion1(uint8 versionMinor)
{
	uint32 leftTocSize = this->chunkHeader.size;
	DataReader dataReader = this->CreateDataReader(this->inputStream);
	while(leftTocSize)
	{
		HeaderEntry headerEntry;

		ChecksumInputStream checksumInputStream(this->inputStream, new TOCEntryChecksumFunction);
		DataReader checkedDataReader = this->CreateDataReader(checksumInputStream);
		TextReader textReader(checksumInputStream, TextCodecType::UTF8);

		uint16 headerEntrySize = checkedDataReader.ReadUInt16();
		leftTocSize -= 2;

		uint16 nProperties = checkedDataReader.ReadUInt16();
		for(uint16 i = 0; i < nProperties; i++)
		{
			uint16 propertyType = checkedDataReader.ReadUInt16();
			uint16 propertySize = checkedDataReader.ReadUInt16();

			this->ReadProperty(propertyType, propertySize, headerEntry, checkedDataReader, textReader);
		}

		uint32 readChecksum = dataReader.ReadUInt32();
		ASSERT_EQUALS(readChecksum, checksumInputStream.Finish());

		leftTocSize -= headerEntrySize;
		this->entries.Push(headerEntry);
	}
}
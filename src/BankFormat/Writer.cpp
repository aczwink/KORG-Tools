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
#include <libkorg/BankFormat/Writer.hpp>
#include "TOCEntryChecksumFunction.hpp"
#include "OC31Compressor.hpp"
//Namespaces
using namespace libKORG::BankFormat;
using namespace StdXX;

//Public methods
UniquePointer<Compressor> Writer::BeginWritingObjectData()
{
	const HeaderEntry& headerEntry = this->headerEntries[this->currentObjectWritingHeaderIndex];
	ObjectStreamFormat format = this->objectFormats[this->currentObjectWritingHeaderIndex];

	Flags<ChunkHeaderFlags> flags = this->IsLeaf(headerEntry.type);
	switch(format)
	{
		case ObjectStreamFormat::Compressed:
			flags.SetFlag(ChunkHeaderFlags::OC31Compressed);
			break;
		case ObjectStreamFormat::Encrypted:
			flags.SetFlag(ChunkHeaderFlags::Encrypted);
			break;
		case ObjectStreamFormat::EncryptedAndCompressed:
			flags.SetFlag(ChunkHeaderFlags::OC31Compressed);
			flags.SetFlag(ChunkHeaderFlags::Encrypted);
			break;
	}

	this->BeginCrossReferencedChunk(this->MapObjectTypeToDataType(headerEntry.type), headerEntry.dataVersion.major, headerEntry.dataVersion.minor, flags);
	if(format == ObjectStreamFormat::Compressed)
		return new OC31Compressor(this->currentObjectBuffer.CreateOutputStream());

	class DelegateCompressor : public Compressor
	{
	public:
		//Constructor
		inline DelegateCompressor(OutputStream& baseStream) : Compressor(baseStream)
		{
		}

		void Flush() override
		{
			this->outputStream.Flush();
		}

		uint32 WriteBytes(const void *source, uint32 size) override
		{
			return this->outputStream.WriteBytes(source, size);
		}
	};
	return new DelegateCompressor(this->outputStream);
}

void Writer::EndWritingObject()
{
	this->currentObjectWritingHeaderIndex++;

	this->currentObjectBuffer.CreateInputStream()->FlushTo(this->outputStream);
	this->currentObjectBuffer.Resize(0);
	this->EndChunk();
}

void Writer::WriteHeader()
{
	this->BeginChunk(ChunkType::Container, 0, 1, ChunkHeaderFlags::Unknown4);

	this->BeginChunk(ChunkType::KorgFile, 0, 0, ChunkHeaderFlags::Leaf);
	this->dataWriter.WriteUInt32(0x0B000000);
	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(0);
	this->fourccWriter.WriteUInt32(FOURCC(u8"KORF"));
	this->dataWriter.WriteByte(0);

	this->EndChunk();
}

void Writer::WriteIndexEntry(const HeaderEntry &headerEntry, ObjectStreamFormat format)
{
	class IndexEntryWriter : public ChunkWriter
	{
	public:
		//Constructor
		inline IndexEntryWriter(SeekableOutputStream& seekableOutputStream) : ChunkWriter(seekableOutputStream)
		{
		}

		//Methods
		void Write(const HeaderEntry &headerEntry, bool encryptionEnabled)
		{
			this->BeginSizeBracket16();

			this->dataWriter.WriteUInt16(encryptionEnabled ? 5 : 3);

			this->dataWriter.WriteUInt16(0);
			this->BeginSizeBracket16();
			this->dataWriter.WriteByte(static_cast<byte>(headerEntry.type));
			this->dataWriter.WriteByte(0);
			this->dataWriter.WriteByte(headerEntry.pos);
			this->EndSizeBracket16();

			this->dataWriter.WriteUInt16(1);
			this->BeginSizeBracket16();
			dataWriter.WriteByte(headerEntry.dataVersion.major);
			dataWriter.WriteByte(headerEntry.dataVersion.minor);
			this->EndSizeBracket16();

			this->dataWriter.WriteUInt16(2);
			this->BeginSizeBracket16();
			TextWriter textWriter(this->outputStream, TextCodecType::UTF8);
			textWriter.WriteString(headerEntry.name);
			this->EndSizeBracket16();

			if(encryptionEnabled)
			{
				this->dataWriter.WriteUInt16(3);
				this->BeginSizeBracket16();
				this->dataWriter.WriteByte(static_cast<byte>(headerEntry.encryptionInformation.serialNumberType));
				this->dataWriter.WriteByte(static_cast<byte>(headerEntry.encryptionInformation.encryptionAlgorithm));
				this->dataWriter.WriteUInt16(headerEntry.encryptionInformation.vendorId);
				this->dataWriter.WriteUInt16(headerEntry.encryptionInformation.featureId);
				this->EndSizeBracket16();

				this->dataWriter.WriteUInt16(4);
				this->BeginSizeBracket16();
				this->dataWriter.WriteUInt64(*headerEntry.id);
				this->EndSizeBracket16();
			}

			this->dataWriter.WriteUInt32(0); //the checksum value... correct value gets written below

			this->EndSizeBracket16();
		}
	};

	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> bufferOutputStream = buffer.CreateOutputStream();

	IndexEntryWriter indexEntryWriter(*bufferOutputStream);
	indexEntryWriter.Write(headerEntry, (format == ObjectStreamFormat::Encrypted) || (format == ObjectStreamFormat::EncryptedAndCompressed));

	UniquePointer<SeekableInputStream> bufferInputStream = buffer.CreateInputStream();
	ChecksumInputStream checksumInputStream(*bufferInputStream, new TOCEntryChecksumFunction);
	checksumInputStream.FlushTo(this->outputStream, buffer.Size() - 4); //exclude the checksum

	//write checksum value
	this->dataWriter.WriteUInt32(checksumInputStream.Finish());

	this->headerEntries.Push(headerEntry);
	this->objectFormats.Push(format);
}

//Private methods
libKORG::ChunkHeaderFlags Writer::IsLeaf(ObjectType objectType) const
{
	switch(objectType)
	{
		case ObjectType::MultiSample:
		case ObjectType::PCM:
		case ObjectType::Sound:
			return ChunkHeaderFlags::Leaf;
	}

	return ChunkHeaderFlags::UnknownAlwaysSetInBankFile;
}

ChunkType Writer::MapObjectTypeToDataType(ObjectType objectType) const
{
	switch(objectType)
	{
		case ObjectType::Performance:
		case ObjectType::StylePerformances:
			return ChunkType::PerformancesData;
		case ObjectType::Style:
			return ChunkType::StyleData;
		case ObjectType::Sound:
			return ChunkType::SoundData;
		case ObjectType::MultiSample:
			return ChunkType::MultiSampleData;
		case ObjectType::PCM:
			return ChunkType::PCMData;
		case ObjectType::Pad:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		case ObjectType::SongBookEntry:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		case ObjectType::SongBook:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	RAISE(ErrorHandling::IllegalCodePathError);
}

void Writer::WriteCrossReferenceTable()
{
	this->BeginChunk(ChunkType::CrossReferenceTable, 0, 0, ChunkHeaderFlags::Leaf);

	this->fourccWriter.WriteUInt32(FOURCC(u8"KBEG"));
	for(uint32 offset : this->crossReferenceObjects)
	{
		this->dataWriter.WriteUInt32(offset);
	}
	this->fourccWriter.WriteUInt32(FOURCC(u8"KEND"));
	this->dataWriter.WriteUInt32(this->crossReferenceObjects.GetNumberOfElements());

	this->EndChunk();
}
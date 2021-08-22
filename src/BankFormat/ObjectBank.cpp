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
#include <libkorg/BankFormat/ObjectBank.hpp>
//Local
#include "ObjectBankWriter.hpp"
//Namespaces
using namespace libKORG;

//Local functions
static BankFormat::ObjectStreamFormat ChunkHeaderToObjectStreamFormat(uint8 flags)
{
	if(flags & (uint8)ChunkHeaderFlags::Encrypted)
	{
		if(flags & (uint8)ChunkHeaderFlags::OC31Compressed)
			return BankFormat::ObjectStreamFormat::EncryptedAndCompressed;
		return BankFormat::ObjectStreamFormat::Encrypted;
	}

	if(flags & (uint8)ChunkHeaderFlags::OC31Compressed)
		return BankFormat::ObjectStreamFormat::Compressed;

	return BankFormat::ObjectStreamFormat::Uncompressed;
}

//Private methods
template<typename ObjectType>
void ObjectBank<ObjectType>::CacheBinaryObject(uint32 dataOffset, UnchangedObjectData& unchangedObjectData)
{
	this->inputStream->SeekTo(dataOffset);
	ChunkHeader chunkHeader;
	auto chunkInputStream = ChunkReader::ReadNextChunk(*this->inputStream, chunkHeader);

	unchangedObjectData.header = chunkHeader;
	auto out = unchangedObjectData.data.CreateOutputStream();
	chunkInputStream->FlushTo(*out);
}

template<typename ObjectType>
BinaryTreeMap<Tuple<uint8, BankFormat::ObjectType>, typename ObjectBank<ObjectType>::UnchangedObjectData> ObjectBank<ObjectType>::CacheBinaryObjects()
{
	BinaryTreeMap<Tuple<uint8, BankFormat::ObjectType>, UnchangedObjectData> unchangedObjects;
	for(const auto& kv : this->objects)
	{
		if(kv.value.hasInputData)
		{
			auto& entry = unchangedObjects[{kv.key, kv.value.headerEntry.type}];
			this->CacheBinaryObject(kv.value.dataOffset, entry);

			if(kv.value.headerEntry.type == BankFormat::ObjectType::Style)
			{
				auto& stsEntry = unchangedObjects[{kv.key, kv.value.stsHeaderEntry.type}];
				this->CacheBinaryObject(kv.value.stsDataOffset, stsEntry);
			}
		}
	}

	return unchangedObjects;
}

template<typename ObjectType>
void ObjectBank<ObjectType>::SaveBank(const StdXX::FileSystem::Path& bankPath)
{
	auto binaryObjectCache = this->CacheBinaryObjects();
	this->inputStream = nullptr;

	StdXX::FileOutputStream fileOutputStream(bankPath, true);
	BankFormat::ObjectBankWriter bankFormatWriter(fileOutputStream, this->model);

	bankFormatWriter.WriteHeader();

	bankFormatWriter.BeginWritingIndex();
	for(const auto& kv : this->objects)
	{
		if(binaryObjectCache.Contains({kv.key, kv.value.headerEntry.type}))
		{
			const auto& entry = binaryObjectCache.Get({kv.key, kv.value.headerEntry.type});
			bankFormatWriter.WriteIndexEntry(kv.value.headerEntry, ChunkHeaderToObjectStreamFormat(entry.header.type));

			if(kv.value.headerEntry.type == BankFormat::ObjectType::Style)
			{
				const auto& stsEntry = binaryObjectCache.Get({kv.key, kv.value.stsHeaderEntry.type});
				bankFormatWriter.WriteIndexEntry(kv.value.stsHeaderEntry, ChunkHeaderToObjectStreamFormat(stsEntry.header.type));
			}
		}
		else
		{
			bankFormatWriter.WriteTOCEntries(kv.value.name, kv.key, *kv.value.object);
		}
	}
	bankFormatWriter.EndIndex();

	for(const auto& kv : this->objects)
	{
		if(binaryObjectCache.Contains({kv.key, kv.value.headerEntry.type}))
		{
			const auto& entry = binaryObjectCache.Get({kv.key, kv.value.headerEntry.type});

			auto compressor = bankFormatWriter.BeginWritingObjectData();
			entry.data.CreateInputStream()->FlushTo(*compressor);
			compressor->Finalize();
			bankFormatWriter.EndWritingObject();

			if(kv.value.headerEntry.type == BankFormat::ObjectType::Style)
			{
				const auto& stsEntry = binaryObjectCache.Get({kv.key, kv.value.stsHeaderEntry.type});

				auto stsCompressor = bankFormatWriter.BeginWritingObjectData();
				stsEntry.data.CreateInputStream()->FlushTo(*stsCompressor);
				stsCompressor->Finalize();
				bankFormatWriter.EndWritingObject();
			}
		}
		else
		{
			bankFormatWriter.WriteObjects(kv.key, *kv.value.object);
		}
	}

	bankFormatWriter.Finalize();
}

//Explicit instantiation
namespace libKORG
{
	template class ObjectBank<AbstractSample>;
	template class ObjectBank<FullStyle>;
	template class ObjectBank<PerformanceObject>;
	template class ObjectBank<SoundObject>;
}
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
#pragma once
//Local
#include <libkorg/ChunkFormat/ChunkWriter.hpp>
#include "BankFormat.hpp"

namespace libKORG::BankFormat
{
	class Writer : protected ChunkWriter
	{
	public:
		//Constructor
		inline Writer(StdXX::SeekableOutputStream &outputStream) : ChunkWriter(outputStream), fourccWriter(false, outputStream)
		{
			this->currentObjectWritingHeaderIndex = 0;
		}

		//Methods
		void WriteHeader();
		void WriteIndexEntry(const HeaderEntry& headerEntry);

		//Inline
		inline void BeginWritingIndex()
		{
			this->BeginCrossReferencedChunk(ChunkType::ObjectTOC, 0, 0, ChunkHeaderFlags::Leaf);
		}

		inline StdXX::OutputStream& BeginWritingObjectData()
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentObjectWritingHeaderIndex++];
			this->BeginCrossReferencedChunk(this->MapObjectTypeToDataType(headerEntry.type), headerEntry.dataVersion.major, headerEntry.dataVersion.minor, this->IsLeaf(headerEntry.type));
			return this->outputStream;
		}

		inline void EndIndex()
		{
			this->EndChunk();
		}

		inline void EndWritingObject()
		{
			this->EndChunk();
		}

		inline void Finalize()
		{
			this->WriteCrossReferenceTable();
			this->EndChunk();
		}

	private:
		//Members
		StdXX::DataWriter fourccWriter;
		StdXX::DynamicArray<uint32> crossReferenceObjects;
		StdXX::DynamicArray<HeaderEntry> headerEntries;
		uint32 currentObjectWritingHeaderIndex;

		//Methods
		ChunkHeaderFlags IsLeaf(ObjectType objectType) const;
		ChunkType MapObjectTypeToDataType(ObjectType objectType) const;
		void WriteCrossReferenceTable();

		//Inline
		inline void BeginChunk(ChunkType type, uint8 versionMajor, uint8 versionMinor, ChunkHeaderFlags flag)
		{
			ChunkWriter::BeginChunk((uint8)type, versionMajor, versionMinor, flag | ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
		}

		inline void BeginCrossReferencedChunk(ChunkType type, uint8 versionMajor, uint8 versionMinor, ChunkHeaderFlags flag)
		{
			this->crossReferenceObjects.Push(this->outputStream.QueryCurrentOffset());
			this->BeginChunk(type, versionMajor, versionMinor, flag);
		}
	};
}
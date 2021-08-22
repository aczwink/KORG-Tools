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
#include "BankFormat.hpp"
#include "BankObject.hpp"

namespace libKORG::BankFormat
{
	struct BankObjectEntry
	{
		HeaderEntry headerEntry;
		uint32 dataOffset;
	};

	class Reader
	{
	public:
		//Methods
		BankObject* ReadBankObject(const HeaderEntry& headerEntry, StdXX::InputStream& inputStream);
		void ReadMetadata(StdXX::SeekableInputStream& seekableInputStream);

		//Inline
		inline StdXX::DynamicArray<libKORG::BankFormat::BankObjectEntry> TakeEntries()
		{
			StdXX::DynamicArray<libKORG::BankFormat::BankObjectEntry> entries;
			for(uint32 i = 0; i < this->headerEntries.GetNumberOfElements(); i++)
			{
				const auto& headerEntry = this->headerEntries[i];

				entries.Push({ headerEntry, this->xref[i + 1] }); //the first entry in the xref is the offset of the TOC
			}

			return entries;
		}

	protected:
		//Overrideable
		virtual ChunkReader* OnEnteringChunkedResourceChunk(const ChunkHeader &chunkHeader, const HeaderEntry& headerEntry);
		virtual BankObject* ReadBankObject(ChunkType chunkType, const ChunkHeader& chunkHeader, const HeaderEntry& headerEntry, StdXX::DataReader& dataReader);

	private:
		//Members
		StdXX::DynamicArray<HeaderEntry> headerEntries;
		StdXX::DynamicArray<uint32> xref;
		StdXX::DynamicArray<BankObjectEntry> objectEntries;
		StdXX::UniquePointer<BankObjectReader> objectReader;

		//Methods
		void FindXRefLocation(StdXX::SeekableInputStream& inputStream);
		void ReadContainerChunkHeader(StdXX::InputStream& inputStream);
		void ReadKorfHeaderChunk(StdXX::InputStream& inputStream);
		void ReadTableOfContentsChunk(StdXX::InputStream& inputStream);
		void ReadXRef(StdXX::InputStream& inputStream);

		//Inline
		inline void VerifyData(const HeaderEntry& headerEntry, const ChunkHeader& chunkHeader)
		{
			this->VerifyDataType(headerEntry, chunkHeader.type);
			this->VerifyDataVersion(headerEntry, chunkHeader.version);
		}

		inline void VerifyDataType(const HeaderEntry& headerEntry, uint8 type)
		{
			switch ((ChunkType)type)
			{
				case ChunkType::PCMData:
					ASSERT_EQUALS(ObjectType::PCM, headerEntry.type);
					break;
				case ChunkType::PerformancesData:
					ASSERT(
							(headerEntry.type == ObjectType::Performance)
							|| (headerEntry.type == ObjectType::StylePerformances), u8"Illegal data");
					break;
				case ChunkType::SoundData:
					ASSERT_EQUALS(ObjectType::Sound, headerEntry.type);
					break;
				case ChunkType::StyleData:
					ASSERT_EQUALS(ObjectType::Style, headerEntry.type);
					break;
			}
		}

		inline void VerifyDataVersion(const HeaderEntry& headerEntry, const ChunkVersion& dataVersion)
		{
			ASSERT_EQUALS(headerEntry.dataVersion.major, dataVersion.major);
			ASSERT_EQUALS(headerEntry.dataVersion.minor, dataVersion.minor);
		}
	};
}
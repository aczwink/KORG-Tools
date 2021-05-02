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
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include <libkorg/Set.hpp>
#include "BankFormat.hpp"

namespace libKORG::BankFormat
{
	class Reader : public ChunkReader
	{
	public:
		//Constructor
		inline Reader()
		{
			this->currentHeaderEntryIndex = 0;
		}

		//Inline
		inline StdXX::DynamicArray<libKORG::BankObjectEntry>&& TakeEntries()
		{
			return Move(this->objectEntries);
		}

	protected:
		//Methods
		ChunkReader* OnEnteringChunk(const ChunkHeader &chunkHeader) override;
		void OnLeavingChunk(const ChunkHeader &chunkHeader) override;
		void ReadDataChunk(const ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;

		//Overrideable
		virtual ChunkReader* OnEnteringChunkedResourceChunk(const ChunkHeader &chunkHeader, const HeaderEntry& headerEntry);
		virtual void ReadBankObject(ChunkType chunkType, const ChunkHeader& chunkHeader, const HeaderEntry& headerEntry, StdXX::DataReader& dataReader);

		//Inline
		inline void Next()
		{
			this->currentHeaderEntryIndex++;
		}

	private:
		//Members
		StdXX::DynamicArray<HeaderEntry> headerEntries;
		uint8 currentHeaderEntryIndex;
		StdXX::DynamicArray<BankObjectEntry> objectEntries;
		StdXX::UniquePointer<BankObjectReader> objectReader;

		//Inline
		inline void AddObject(BankObject* object, const HeaderEntry& headerEntry)
		{
			this->objectEntries.Push({ headerEntry.name, headerEntry.pos, object });
		}

		inline void VerifyData(const ChunkHeader& chunkHeader)
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex];
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
				case ChunkType::StyleObject:
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
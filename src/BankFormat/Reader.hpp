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
#include <libkorg/Set.hpp>
#include "BankFormat.hpp"
#include "libkorg/BankObject.hpp"
#include "libkorg/ChunkFormat.hpp"
#include "../Reading/ChunkReader.hpp"

namespace BankFormat
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
		StdXX::String GetDebugDirName() const override;
		bool IsDataChunk(const ChunkHeader &chunkHeader) override;
		void ReadDataChunk(const ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;

	private:
		//Members
		StdXX::DynamicArray<HeaderEntry> headerEntries;
		uint8 currentHeaderEntryIndex;
		StdXX::DynamicArray<BankObjectEntry> objectEntries;

		//Inline
		inline void AddObject(BankObject* object)
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex++];
			this->objectEntries.Push({ headerEntry.name, headerEntry.pos, object });
		}

		inline void VerifyDataVersion(const ChunkVersion& dataVersion)
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex];
			ASSERT_EQUALS(headerEntry.dataVersion.major, dataVersion.major);
			ASSERT_EQUALS(headerEntry.dataVersion.minor, dataVersion.minor);
		}
	};
}
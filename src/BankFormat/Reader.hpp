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
#include "BankFormat.hpp"
#include "libkorg/BankObject.hpp"
#include "libkorg/ChunkFormat.hpp"
#include "../Reading/ChunkReader.hpp"

struct BankObjectEntry
{
	String name;
	uint8 pos;
	BankObject* object;
};

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
		inline DynamicArray<BankObjectEntry>&& TakeEntries()
		{
			return Move(this->objectEntries);
		}

	protected:
		//Methods
		String GetDebugDirName() const override;
		bool IsDataChunk(const ChunkHeader &chunkHeader) override;
		void ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader) override;

	private:
		//Members
		DynamicArray<HeaderEntry> headerEntries;
		uint8 currentHeaderEntryIndex;
		DynamicArray<BankObjectEntry> objectEntries;

		//Inline
		inline void AddObject(BankObject* object)
		{
			const HeaderEntry& headerEntry = this->headerEntries[this->currentHeaderEntryIndex++];
			this->objectEntries.Push({ headerEntry.name, headerEntry.pos, object });
		}
	};
}
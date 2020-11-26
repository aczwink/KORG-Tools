/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "BankObject.hpp"

struct BankObjectEntry
{
	String name;
	uint8 pos;
	BankObject* object;
};

class BankFormatReader
{
public:
	//Constructor
	inline BankFormatReader(SeekableInputStream& inputStream) : inputStream(inputStream), dataReader(true, inputStream)
	{
	}

	//Methods
	DynamicArray<BankObjectEntry> Read();

private:
	//Members
	SeekableInputStream& inputStream;
	DataReader dataReader;
	DynamicArray<BankObjectEntry> objectEntries;

	//Methods
	KorgFormat::ChunkHeader ReadChunkHeader();
	void ReadEntries(const DynamicArray<KorgFormat::HeaderEntry>& headerEntries);
	DynamicArray<KorgFormat::HeaderEntry> ReadTOC();
	void ReadHeader();

	//Inline
	inline DataReader CreateFourCCReader(StdXX::InputStream& inputStream)
	{
		return DataReader(false, inputStream);
	}
};
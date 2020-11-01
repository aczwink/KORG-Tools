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
#include "StyleBank.hpp"
#include "StyleBankFormat.hpp"

class KorgFormatReader
{
public:
	//Constructor
	inline KorgFormatReader(SeekableInputStream& inputStream) : inputStream(inputStream), dataReader(true, inputStream)
	{
	}

	//Methods
	StyleBank Read();

private:
	//Members
	SeekableInputStream& inputStream;
	DataReader dataReader;

	//Methods
	KorgFormat::ChunkHeader ReadChunkHeader();
	void ReadEntries(const DynamicArray<KorgFormat::HeaderEntry>& headerEntries, StyleBank& styleBank);
	DynamicArray<KorgFormat::HeaderEntry> ReadTOC();
	DynamicArray<KorgFormat::HeaderEntry> ReadTOC_Extended(uint32 tocSize);
	void ReadHeader();

	//Inline
	inline DataReader CreateFourCCReader()
	{
		return DataReader(false, this->inputStream);
	}
};
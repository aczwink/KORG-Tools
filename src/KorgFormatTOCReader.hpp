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
#include <libkorg/BankFormat.hpp>

class KorgFormatTOCReader
{
public:
	//Constructor
	inline KorgFormatTOCReader(uint32 tocId, uint32 tocSize, InputStream& inputStream, DataReader& dataReader)
		: inputStream(inputStream), dataReader(dataReader), textReader(inputStream, TextCodecType::ASCII)
	{
		this->tocId = tocId;
		this->leftTocSize = tocSize;
	}

	//Properties
	inline const DynamicArray<KorgFormat::HeaderEntry>& Entries() const
	{
		return this->entries;
	}

	//Methods
	void Read();

private:
	//Members
	uint32 tocId;
	uint32 leftTocSize;
	InputStream& inputStream;
	DataReader& dataReader;
	TextReader textReader;
	DynamicArray<KorgFormat::HeaderEntry> entries;

	//Methods
	KorgFormat::HeaderEntry ReadEntryVersion3();
	KorgFormat::HeaderEntry ReadEntryVersion5();
	void ReadExtended();
	void ReadExtended2();
	void ReadStandard();
};
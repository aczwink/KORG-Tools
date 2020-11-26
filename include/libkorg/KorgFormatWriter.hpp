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
#include <StdXX.hpp>
using namespace StdXX;
//Local
#include "ObjectBank.hpp"

class KorgFormatWriter
{
public:
	//Constructor
	inline KorgFormatWriter(SeekableOutputStream& outputStream) : outputStream(outputStream),
																  dataWriter(true, outputStream), fourccWriter(false, outputStream)
	{
	}

	//Methods
	//void Write(ObjectBank& styleBank);

private:
	//Members
	SeekableOutputStream& outputStream;
	DataWriter dataWriter;
	DataWriter fourccWriter;
	DynamicArray<uint32> dataObjectOffsets;

	//Methods
	void WriteHeader(uint32 numberOfEntries);
	//void WriteHeaderEntries(const ObjectBank& styleBank);
	//void WriteObjects(const ObjectBank& styleBank);
	void WriteTOC();
};
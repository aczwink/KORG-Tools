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
//Local
#include <libkorg/ChunkFormat.hpp>
#include "BankFormat.hpp"
using namespace libKORG;

namespace BankFormat
{
	class TOCReader
	{
	public:
		//Constructor
		inline TOCReader(const ChunkHeader& chunkHeader, StdXX::DataReader& dataReader)
			: chunkHeader(chunkHeader), dataReader(dataReader), textReader(dataReader.InputStream(), TextCodecType::ASCII)
		{
		}

		//Methods
		DynamicArray<HeaderEntry> Read();

	private:
		//Members
		const ChunkHeader& chunkHeader;
		StdXX::DataReader& dataReader;
		StdXX::TextReader textReader;
		DynamicArray<BankFormat::HeaderEntry> entries;

		//Methods
		HeaderEntry ReadEntryVersion1_3();
		HeaderEntry ReadEntryVersion1_4();
		HeaderEntry ReadEntryVersion1_5();
		void ReadEntryVersion1_General(HeaderEntry& headerEntry);
		void ReadVersion0();
		void ReadVersion1(uint8 versionMinor);
	};
}
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
#include <libkorg/ChunkFormat/ChunkFormat.hpp>
#include <libkorg/BankFormat/BankFormat.hpp>
using namespace libKORG;


	class TOCReader
	{
	public:
		//Constructor
		inline TOCReader(const ChunkHeader& chunkHeader, StdXX::InputStream& inputStream)
			: chunkHeader(chunkHeader), inputStream(inputStream)
		{
		}

		//Methods
		StdXX::DynamicArray<libKORG::BankFormat::HeaderEntry> Read();

	private:
		//Members
		const ChunkHeader& chunkHeader;
		StdXX::InputStream& inputStream;
		StdXX::DynamicArray<libKORG::BankFormat::HeaderEntry> entries;

		//Methods
		libKORG::BankFormat::ObjectType ReadObjectType(StdXX::DataReader& dataReader);
		void ReadProperty(uint16 propertyType, uint16 propertySize, libKORG::BankFormat::HeaderEntry& headerEntry, StdXX::DataReader& dataReader, StdXX::TextReader& textReader);
		void ReadVersion0(StdXX::DataReader& dataReader, StdXX::TextReader& textReader);
		void ReadVersion1(uint8 versionMinor);

		//Inline
		inline StdXX::DataReader CreateDataReader(StdXX::InputStream& inputStream)
		{
			return StdXX::DataReader(true, inputStream);
		}
	};
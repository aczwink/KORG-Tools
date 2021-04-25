/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg/BankObject.hpp>
#include <libkorg/ChunkFormat/ChunkFormat.hpp>

class PCMReader
{
public:
	AbstractSample *Read(const libKORG::ChunkHeader& chunkHeader, StdXX::DataReader& dataReader)
	{
		ASSERT_EQUALS(0, chunkHeader.version.major);
		ASSERT_EQUALS(0, chunkHeader.version.minor);

		if(chunkHeader.flags & (uint8)ChunkHeaderFlags::Encrypted)
			return new EncryptedSample(dataReader.InputStream());

		return new Sample(dataReader.InputStream());
	}
};
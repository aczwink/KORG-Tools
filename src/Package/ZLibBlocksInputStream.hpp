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
#include <StdXX.hpp>

/*
 * The block structure and in particular block 0x100 are based on the info provided by Mateusz Faderewski (https://github.com/Polprzewodnikowy/KorgPackage)
 */

class ZLibBlocksInputStream : public StdXX::BlockInputStream<2 * MiB>
{
public:
	//Constructor
	inline ZLibBlocksInputStream(StdXX::InputStream& baseInputStream, bool verify, uint64 dataSize, uint64 fileSize) : BlockInputStream(baseInputStream)
	{
		this->verify = verify;
		this->endBlockRead = false;

		this->dataSize = dataSize;
		this->fileSize = fileSize;
		this->readDataSize = 0;
		this->readFileSize = 0;
	}

	//Methods
	bool IsAtEnd() const override
	{
		return this->endBlockRead || BlockInputStream::IsAtEnd();
	}

private:
	//Members
	bool verify;
	bool endBlockRead;
	uint64 dataSize;
	uint64 fileSize;
	uint64 readDataSize;
	uint64 readFileSize;

	//Methods
	uint32 ReadEndOfStreamBlock(StdXX::DataReader& dataReader, uint64 from, uint64 to);
	uint32 ReadNextBlock(StdXX::InputStream& baseInputStream, void* destination) override;
	uint32 ReadZLibBlock(StdXX::DataReader& dataReader, void* destination);
};
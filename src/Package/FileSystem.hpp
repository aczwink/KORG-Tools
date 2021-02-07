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
#include "PackageInternals.hpp"
#include "ZLibBlocksInputStream.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

struct PackageFileHeader : public ContainerFileHeader
{
	FileDataType dataType;
	byte readMD5[16];

	inline PackageFileHeader()
	{
		this->type = FileType::File;
	}
};

class PackageFileSystem : public CustomArchiveFileSystem<PackageFileHeader>
{
public:
	//Constructor
	inline PackageFileSystem(SeekableInputStream& seekableInputStream) : CustomArchiveFileSystem(seekableInputStream)
	{
	}

	using CustomArchiveFileSystem::AddSourceFile;
	using CustomArchiveFileSystem::AddSourceDirectory;

private:
	void AddTypedFilters(ChainedInputStream& chainedInputStream, const PackageFileHeader &header, bool verify) const override
	{
		switch(header.dataType)
		{
			case FILE_DATA_TYPE_RAW:
				break;
			case FILE_DATA_TYPE_ZLIB_BLOCKS:
				chainedInputStream.Add(new ZLibBlocksInputStream(chainedInputStream.GetEnd(), verify, header.storedSize, header.size));
				break;
			default:
			case FILE_DATA_TYPE_ENCRYPTED:
				NOT_IMPLEMENTED_ERROR;
		}

		if(verify)
		{
			StdXX::FixedSizeBuffer buffer(header.readMD5, sizeof(header.readMD5));
			chainedInputStream.Add(new StdXX::Crypto::CheckedHashingInputStream(chainedInputStream.GetEnd(), StdXX::Crypto::HashAlgorithm::MD5, buffer));
		}
	}
};
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

struct FileHeaderInfo
{
	byte readMD5[16];
	FileDataType fileDataType;
	ContainerFileHeader header;
};

class PackageFile : public StdXX::FileSystem::ContainerFile
{
public:
	inline PackageFile(const FileHeaderInfo &header, ContainerFileSystem *fileSystem) : ContainerFile(header.header, fileSystem)
	{
		MemCopy(this->readMD5, header.readMD5, sizeof(this->readMD5));
		this->fileDataType = header.fileDataType;
	}

	StdXX::UniquePointer<StdXX::InputStream> OpenForReading(bool verify) const override
	{
		StdXX::ChainedInputStream* chain = new StdXX::ChainedInputStream(ContainerFile::OpenForReading(verify));

		switch(this->fileDataType)
		{
			case FILE_DATA_TYPE_RAW:
				break;
			case FILE_DATA_TYPE_ZLIB_BLOCKS:
				chain->Add(new ZLibBlocksInputStream(chain->GetEnd(), verify, this->GetHeader().compressedSize, this->GetHeader().uncompressedSize));
				break;
			default:
			case FILE_DATA_TYPE_ENCRYPTED:
				NOT_IMPLEMENTED_ERROR;
		}

		if(verify)
		{
			StdXX::FixedSizeBuffer buffer(this->readMD5, sizeof(this->readMD5));
			chain->Add(new StdXX::Crypto::CheckedHashingInputStream(chain->GetEnd(), StdXX::Crypto::HashAlgorithm::MD5, buffer));
		}
		return chain;
	}

private:
	//Members
	FileDataType fileDataType;
	byte readMD5[16];
};

class PackageFileSystem : public FileSystem::ContainerFileSystem
{
public:
	PackageFileSystem(const Path &fileSystemPath) : ContainerFileSystem(fileSystemPath)
	{
	}

	using FileSystem::ContainerFileSystem::AddSourceDirectory;
	using ContainerFileSystem::AddSourceFile;

	void Flush() override
	{
		NOT_IMPLEMENTED_ERROR;
	}
};
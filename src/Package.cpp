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
//Class header
#include <libkorg/Package.hpp>
//Local
#include "Package/FileSystem.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

/*
 * The overall structure of the package files was implemented based on the info provided by Mateusz Faderewski (https://github.com/Polprzewodnikowy/KorgPackage)
 */

class PackageReader
{
	struct FileData
	{
		Path path;
		PackageFileHeader header;
	};
public:
	//Members
	PackageHeader header;
	UniquePointer<PackageFileSystem> fs;

	//Constructor
	PackageReader(SeekableInputStream &inputStream) : inputStream(inputStream)
	{
	}

	//Methods
	void ReadData()
	{
		FixedSizeBuffer overallFileMd5(16);
		this->inputStream.ReadBytes(overallFileMd5.Data(), overallFileMd5.Size());

		BufferedInputStream bufferedInputStream(this->inputStream);
		Crypto::CheckedHashingInputStream checkedHashingInputStream(bufferedInputStream, Crypto::HashAlgorithm::MD5, overallFileMd5);

		try
		{
			this->ReadChunks(checkedHashingInputStream, this->inputStream.QueryCurrentOffset());
		}
		catch(ErrorHandling::VerificationFailedException)
		{
			stdErr << u8"Package file is corrupted. Expect errors." << endl;
		}

		this->GenerateFileSystem();
	}

private:
	//Members
	SeekableInputStream &inputStream;
	DynamicArray<String> directoryPaths;
	Map<String, UniquePointer<FileSystem::POSIXPermissions>> directoryPermissions;
	DynamicArray<FileData> filesData;

	//Methods
	void GenerateFileSystem()
	{
		this->fs = new PackageFileSystem(inputStream);

		this->directoryPaths.Sort();

		for (const String& path : this->directoryPaths)
		{
			FileInfo fileInfo;
			fileInfo.type = FileType::Directory;
			fileInfo.permissions = Move(this->directoryPermissions[path]);

			this->fs->AddSourceDirectory(path, Move(fileInfo));
		}

		for(const FileData& fileData : this->filesData)
		{
			if(fileData.header.dataType == FILE_DATA_TYPE_ENCRYPTED)
			{
				stdErr << u8"Skipping encrypted file: " << fileData.path << endl;
				continue;
			}

			Path absPath = fileData.path;
			if(!absPath.IsAbsolute())
				absPath = u8"/" + fileData.path.String();

			this->fs->AddSourceFile(absPath, fileData.header);
		}
	}

	void ReadChunks(InputStream& inputStream, uint64 offset)
	{
		DataReader dataReader(false, inputStream);
		while(!dataReader.InputStream().IsAtEnd())
		{
			uint32 chunkId = dataReader.ReadUInt32();
			uint32 chunkSize = dataReader.ReadUInt32();
			offset += 8;

			switch(chunkId)
			{
				case PACKAGE_CHUNK_ID_PACKAGE_HEADER:
					this->ReadPackageHeader(dataReader);
					break;
				case PACKAGE_CHUNK_ID_UPDATE_KERNEL:
				case PACKAGE_CHUNK_ID_UPDATE_RAMDISK:
				case PACKAGE_CHUNK_ID_UPDATE_INSTALLER:
				case PACKAGE_CHUNK_ID_UPDATE_INSTALLER_CONFIG:
				case PACKAGE_CHUNK_ID_USER_KERNEL:
					this->ReadSystemFile(chunkId, chunkSize, dataReader, offset);
					break;
				case PACKAGE_CHUNK_ID_INSTALLER_SCRIPT:
					this->ReadInstallerScript(dataReader, chunkSize, offset);
					break;
				case PACKAGE_CHUNK_ID_DIRECTORY:
					this->ReadDirectory(dataReader);
					break;
				case PACKAGE_CHUNK_ID_FILE:
					this->ReadFile(dataReader, chunkSize, offset);
					break;
				case PACKAGE_CHUNK_ID_ROOTFS:
					this->ReadRootFS(dataReader, offset);
					break;
				default:
					stdErr << u8"Skipping unknown chunk: " << String::HexNumber(chunkId) << endl;
					dataReader.Skip(chunkSize);
			}

			offset += chunkSize;
			uint64 remainder = offset % 4;
			if(remainder)
			{
				uint64 padding = 4 - remainder;
				dataReader.Skip(padding);
				offset += padding;
			}
		}
	}

	void ReadDirectory(DataReader& dataReader)
	{
		UniquePointer<FileSystem::POSIXPermissions> permissions = this->ReadPermissions(dataReader);
		dataReader.ReadUInt16(); //condition?

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
		String path = textReader.ReadZeroTerminatedString();

		this->directoryPaths.Push(path); //paths are unfortunately out of order
		this->directoryPermissions.Insert(path, Move(permissions));
	}

	void ReadFile(DataReader& dataReader, uint32 chunkSize, uint64 offset)
	{
		FileData fileData;

		dataReader.ReadBytes(fileData.header.readMD5, sizeof(fileData.header.readMD5));
		fileData.header.permissions = this->ReadPermissions(dataReader);
		dataReader.ReadUInt16(); //condition?

		fileData.header.size = dataReader.ReadUInt32();
		fileData.header.dataType = static_cast<FileDataType>(dataReader.ReadByte());

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		fileData.path = textReader.ReadZeroTerminatedString();
		String date = textReader.ReadZeroTerminatedString();
		String time = textReader.ReadZeroTerminatedString();

		uint32 fileHeaderSize = 29 + (fileData.path.String().GetLength() + 1) + (date.GetLength() + 1) + (time.GetLength() + 1);
		fileData.header.storedSize = chunkSize - fileHeaderSize;

		fileData.header.offset = offset + fileHeaderSize;

		this->filesData.Push(fileData);

		dataReader.Skip(fileData.header.storedSize);
	}

	void ReadInstallerScript(DataReader& dataReader, uint32 chunkSize, uint64 offset)
	{
		FileData fileData;
		PackageFileHeader& header = fileData.header;

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		dataReader.ReadBytes(header.readMD5, sizeof(header.readMD5));
		dataReader.ReadUInt16(); //condition?
		fileData.path = textReader.ReadZeroTerminatedString();

		uint32 headerSize = sizeof(header.readMD5) + 2 + (fileData.path.String().GetLength() + 1);

		header.dataType = FILE_DATA_TYPE_RAW;
		header.size = chunkSize - headerSize;
		header.storedSize = header.size;
		header.offset = offset + headerSize;

		this->filesData.Push(fileData);

		dataReader.Skip(header.storedSize);
	}

	void ReadPackageHeader(DataReader& dataReader)
	{
		dataReader.ReadBytes(this->header.pkgLibraryVersion, sizeof(this->header.pkgLibraryVersion));

		uint32 pkgType = dataReader.ReadUInt32();
		ASSERT((pkgType == 2)
			|| (pkgType == 3)
			|| (pkgType == 4), String::Number(pkgType));
		dataReader.ReadUInt16();
		dataReader.ReadUInt16();

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		this->header.systemType = textReader.ReadZeroTerminatedString();
		this->header.buildSystem1 = textReader.ReadZeroTerminatedString();
		this->header.buildSystem2 = textReader.ReadZeroTerminatedString();
		this->header.creationDate = textReader.ReadZeroTerminatedString();
		this->header.creationTime = textReader.ReadZeroTerminatedString();
		this->header.packageType1 = textReader.ReadZeroTerminatedString();
		this->header.packageType2 = textReader.ReadZeroTerminatedString();
	}

	UniquePointer<FileSystem::POSIXPermissions> ReadPermissions(DataReader& dataReader)
	{
		uint16 owner = dataReader.ReadUInt16();
		uint16 group = dataReader.ReadUInt16();
		uint16 attributes = dataReader.ReadUInt16();

		UniquePointer<FileSystem::POSIXPermissions> perm = new FileSystem::POSIXPermissions(owner, group, 0);
		perm->others.execute = (attributes & ATTR_EXT3_OTHER_X) != 0;
		perm->others.read = (attributes & ATTR_EXT3_OTHER_R) != 0;
		perm->others.write = (attributes & ATTR_EXT3_OTHER_W) != 0;

		perm->group.execute = (attributes & ATTR_EXT3_GROUP_X) != 0;
		perm->group.read = (attributes & ATTR_EXT3_GROUP_R) != 0;
		perm->group.write = (attributes & ATTR_EXT3_GROUP_W) != 0;

		perm->owner.execute = (attributes & ATTR_EXT3_OWNER_X) != 0;
		perm->owner.read = (attributes & ATTR_EXT3_OWNER_R) != 0;
		perm->owner.write = (attributes & ATTR_EXT3_OWNER_W) != 0;

		return perm;
	}

	void ReadRootFS(DataReader& dataReader, uint64 offset)
	{
		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		FileData fileData;
		PackageFileHeader& header = fileData.header;

		dataReader.ReadBytes(header.readMD5, sizeof(header.readMD5));
		header.storedSize = dataReader.ReadUInt32();
		dataReader.ReadUInt16(); //condition?
		fileData.path = textReader.ReadZeroTerminatedString();

		uint32 headerSize = sizeof(header.readMD5) + 6 + (fileData.path.String().GetLength() + 1);

		header.size = header.storedSize;
		header.offset = offset + headerSize;
		header.dataType = FILE_DATA_TYPE_RAW;

		this->filesData.Push(fileData);

		dataReader.Skip(header.storedSize);
	}

	void ReadSystemFile(uint32 chunkId, uint32 chunkSize, DataReader& dataReader, uint64 offset)
	{
		String path;
		switch(chunkId)
		{
			case PACKAGE_CHUNK_ID_UPDATE_KERNEL:
				path = u8"/update/uImage";
				break;
			case PACKAGE_CHUNK_ID_UPDATE_RAMDISK:
				path = u8"/update/ramdisk.gz";
				break;
			case PACKAGE_CHUNK_ID_UPDATE_INSTALLER:
				path = u8"/update/lfo-pkg-install";
				break;
			case PACKAGE_CHUNK_ID_UPDATE_INSTALLER_CONFIG:
				path = u8"/update/lfo-pkg-install.xml";
				break;
			case PACKAGE_CHUNK_ID_USER_KERNEL:
				path = u8"/kernel/uImage";
				break;
		}

		FileData fileData;
		PackageFileHeader& header = fileData.header;

		fileData.path = path;
		header.dataType = FILE_DATA_TYPE_RAW;
		dataReader.ReadBytes(header.readMD5, sizeof(header.readMD5));
		header.storedSize = chunkSize - sizeof(header.readMD5);
		header.offset = offset + sizeof(header.readMD5);
		header.size = header.storedSize;

		this->filesData.Push(fileData);

		dataReader.Skip(header.storedSize);
	}
};

//Class functions
UniquePointer <Package> libKORG::Package::ReadPackage(SeekableInputStream &inputStream)
{
	PackageReader packageReader(inputStream);
	packageReader.ReadData();

	Package* package = new Package;
	package->header = packageReader.header;
	package->fileSystem = Move(packageReader.fs);

	return package;
}

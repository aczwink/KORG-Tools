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
		FileDataType dataType;
		uint64 dataOffset;
		uint32 fileSize;
		uint32 dataSize;
		byte readMD5[16];
		SharedPointer<FileSystem::POSIXPermissions> permissions;
	};
public:
	//Members
	PackageHeader header;
	UniquePointer<PackageFileSystem> fs;

	//Constructor
	PackageReader(SeekableInputStream &inputStream) : inputStream(inputStream)
	{
		//TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		this->fs = new PackageFileSystem(p);
		//TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
			this->ReadChunks(checkedHashingInputStream, this->inputStream.GetCurrentOffset());
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
	Map<String, SharedPointer<FileSystem::POSIXPermissions>> directoryPermissions;
	DynamicArray<FileData> filesData;

	//Methods
	void GenerateFileSystem()
	{
		this->directoryPaths.Sort();

		for (const String& path : this->directoryPaths)
		{
			this->fs->AddSourceDirectory(path);
			this->fs->GetDirectory(path)->ChangePermissions(*this->directoryPermissions[path]);
		}

		for(const FileData& fileData : this->filesData)
		{
			ContainerFileHeader header;
			header.compressedSize = fileData.dataSize;
			header.uncompressedSize = fileData.fileSize;
			header.offset = fileData.dataOffset;

			if(fileData.dataType == FILE_DATA_TYPE_ENCRYPTED)
			{
				stdErr << u8"Skipping encrypted file: " << fileData.path << endl;
				continue;
			}

			FileHeaderInfo fileHeaderInfo;
			fileHeaderInfo.fileDataType = fileData.dataType;
			fileHeaderInfo.header = header;
			MemCopy(fileHeaderInfo.readMD5, fileData.readMD5, sizeof(fileHeaderInfo.readMD5));

			Path absPath = fileData.path;
			if(!absPath.IsAbsolute())
				absPath = u8"/" + fileData.path.String();

			this->fs->AddSourceFile(absPath, new PackageFile(fileHeaderInfo, this->fs.operator->()));
			if(!fileData.permissions.IsNull())
				this->fs->GetNode(absPath)->ChangePermissions(*fileData.permissions);
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
		SharedPointer<FileSystem::POSIXPermissions> permissions = this->ReadPermissions(dataReader);
		dataReader.ReadUInt16(); //condition?

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);
		String path = textReader.ReadZeroTerminatedString();

		this->directoryPaths.Push(path); //paths are unfortunately out of order
		this->directoryPermissions[path] = permissions;
	}

	void ReadFile(DataReader& dataReader, uint32 chunkSize, uint64 offset)
	{
		FileData fileData;

		dataReader.ReadBytes(fileData.readMD5, sizeof(fileData.readMD5));
		fileData.permissions = this->ReadPermissions(dataReader);
		dataReader.ReadUInt16(); //condition?

		fileData.fileSize = dataReader.ReadUInt32();
		fileData.dataType = static_cast<FileDataType>(dataReader.ReadByte());

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		fileData.path = textReader.ReadZeroTerminatedString();
		String date = textReader.ReadZeroTerminatedString();
		String time = textReader.ReadZeroTerminatedString();

		uint32 fileHeaderSize = 29 + (fileData.path.String().GetLength() + 1) + (date.GetLength() + 1) + (time.GetLength() + 1);
		fileData.dataSize = chunkSize - fileHeaderSize;

		fileData.dataOffset = offset + fileHeaderSize;

		this->filesData.Push(fileData);

		dataReader.Skip(fileData.dataSize);
	}

	void ReadInstallerScript(DataReader& dataReader, uint32 chunkSize, uint64 offset)
	{
		FileData fileData;

		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		dataReader.ReadBytes(fileData.readMD5, sizeof(fileData.readMD5));
		dataReader.ReadUInt16(); //condition?
		fileData.path = textReader.ReadZeroTerminatedString();

		uint32 headerSize = sizeof(fileData.readMD5) + 2 + (fileData.path.String().GetLength() + 1);

		fileData.dataType = FILE_DATA_TYPE_RAW;
		fileData.fileSize = chunkSize - headerSize;
		fileData.dataSize = fileData.fileSize;
		fileData.dataOffset = offset + headerSize;

		this->filesData.Push(fileData);

		dataReader.Skip(fileData.dataSize);
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

	SharedPointer<FileSystem::POSIXPermissions> ReadPermissions(DataReader& dataReader)
	{
		uint16 owner = dataReader.ReadUInt16();
		uint16 group = dataReader.ReadUInt16();
		uint16 attributes = dataReader.ReadUInt16();

		//TODO: ATTRIBUTES TO MODE
		//TODO: check fuse
		return new FileSystem::POSIXPermissions(owner, group, attributes);
	}

	void ReadRootFS(DataReader& dataReader, uint64 offset)
	{
		TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

		FileData fileData;
		dataReader.ReadBytes(fileData.readMD5, sizeof(fileData.readMD5));
		fileData.dataSize = dataReader.ReadUInt32();
		dataReader.ReadUInt16(); //condition?
		fileData.path = textReader.ReadZeroTerminatedString();

		uint32 headerSize = sizeof(fileData.readMD5) + 6 + (fileData.path.String().GetLength() + 1);

		fileData.fileSize = fileData.dataSize;
		fileData.dataOffset = offset + headerSize;
		fileData.dataType = FILE_DATA_TYPE_RAW;

		this->filesData.Push(fileData);

		dataReader.Skip(fileData.dataSize);
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

		fileData.path = path;
		fileData.dataType = FILE_DATA_TYPE_RAW;
		dataReader.ReadBytes(fileData.readMD5, sizeof(fileData.readMD5));
		fileData.dataSize = chunkSize - sizeof(fileData.readMD5);
		fileData.dataOffset = offset + sizeof(fileData.readMD5);
		fileData.fileSize = fileData.dataSize;

		this->filesData.Push(fileData);

		dataReader.Skip(fileData.dataSize);
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

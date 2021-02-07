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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX::FileSystem;

void PrintHeaderInfo(const PackageHeader& header)
{
	stdOut << u8"Package library version: " << header.pkgLibraryVersion[0]
		   << u8"." << header.pkgLibraryVersion[1]
		   << u8"." << header.pkgLibraryVersion[2]
		   << u8"." << header.pkgLibraryVersion[3]
		   << endl;

	stdOut << header.systemType << endl;
	stdOut << header.buildSystem1 << endl;
	stdOut << header.buildSystem2 << endl;
	stdOut << header.creationDate << endl;
	stdOut << header.creationTime << endl;
	stdOut << header.packageType1 << endl;
	stdOut << header.packageType2 << endl;
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	OSFileSystem& osFileSystem = FileSystemsManager::Instance().OSFileSystem();
	Path pkgPath = osFileSystem.FromNativePath(args[0]);
	FileInputStream fileInputStream(pkgPath);
	UniquePointer<Package> package = Package::ReadPackage(fileInputStream);

	PrintHeaderInfo(package->header);

	Path mntPath = osFileSystem.FromNativePath(args[1]);
	osFileSystem.MountReadOnly(mntPath, *package->fileSystem);

	return EXIT_SUCCESS;
}
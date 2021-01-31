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

namespace libKORG
{
	struct PackageHeader
	{
		byte pkgLibraryVersion[4];
		//mach_id and customization
		StdXX::String systemType;
		StdXX::String buildSystem1;
		StdXX::String buildSystem2;
		StdXX::String creationDate;
		StdXX::String creationTime;
		StdXX::String packageType1;
		StdXX::String packageType2;
	};

	class Package
	{
	public:
		//Members
		PackageHeader header;
		StdXX::UniquePointer<StdXX::FileSystem::ReadableFileSystem> fileSystem;

		//Functions
		static StdXX::UniquePointer<Package> ReadPackage(StdXX::SeekableInputStream& inputStream);
	};
}
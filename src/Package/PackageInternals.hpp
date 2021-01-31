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

/*
 * All values in the below enums where found by Mateusz Faderewski (https://github.com/Polprzewodnikowy/KorgPackage)
 */

enum PackageChunkId
{
	PACKAGE_CHUNK_ID_PACKAGE_HEADER = 1,
	PACKAGE_CHUNK_ID_UPDATE_KERNEL = 2,
	PACKAGE_CHUNK_ID_UPDATE_RAMDISK = 3,
	PACKAGE_CHUNK_ID_UPDATE_INSTALLER = 4,
	PACKAGE_CHUNK_ID_UPDATE_INSTALLER_CONFIG = 5,
	PACKAGE_CHUNK_ID_USER_KERNEL = 14,
	PACKAGE_CHUNK_ID_INSTALLER_SCRIPT = 15,
	PACKAGE_CHUNK_ID_DIRECTORY = 16,
	PACKAGE_CHUNK_ID_FILE = 17,
	PACKAGE_CHUNK_ID_ROOTFS = 19,
};

enum FileDataType
{
	FILE_DATA_TYPE_RAW = 0,
	FILE_DATA_TYPE_ZLIB_BLOCKS = 1,
	FILE_DATA_TYPE_ENCRYPTED = 16,
};

enum FileNodeAttributes
{
	ATTR_VFAT_ARCHIVE = 0x1000,
	ATTR_VFAT_READONLY = 0x2000,
	ATTR_VFAT_SYSTEM = 0x4000,
	ATTR_VFAT_HIDDEN = 0x8000,
	ATTR_EXT3_OWNER_R = 0x0040,
	ATTR_EXT3_OWNER_W = 0x0080,
	ATTR_EXT3_OWNER_X = 0x0100,
	ATTR_EXT3_GROUP_R = 0x0008,
	ATTR_EXT3_GROUP_W = 0x0010,
	ATTR_EXT3_GROUP_X = 0x0020,
	ATTR_EXT3_OTHER_R = 0x0001,
	ATTR_EXT3_OTHER_W = 0x0002,
	ATTR_EXT3_OTHER_X = 0x0004,
};
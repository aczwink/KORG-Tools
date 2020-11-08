/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg/Set.hpp>
//Local
#include <libkorg/KorgFormatReader.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
Set::Set(const Path &setPath)
{
	//this->ReadDirectory(setPath, u8"PCM");
	this->ReadDirectory(setPath, u8"STYLE");
}

//Private methods
void Set::ReadDirectory(const Path &setPath, const String &dirName)
{
	Path dirPath = setPath / dirName;
	AutoPointer<Directory> directory = OSFileSystem::GetInstance().GetDirectory(dirPath);
	for (const String& childName : *directory)
	{
		FileInputStream fileInputStream(dirPath / childName);

		KorgFormatReader korgFormatReader(fileInputStream);
		StyleBank styleBank = korgFormatReader.Read();
	}
}
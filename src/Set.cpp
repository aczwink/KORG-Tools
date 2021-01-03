/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg/BankFormatReader.hpp>
#include <libkorg/SingleTouchSettings.hpp>
#include <libkorg/Text.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
Set::Set(const Path &setPath)
{
	//TODO: GLOBAL
	//this->ReadDirectory(setPath, u8"MULTISMP", &Set::LoadMultiSamples);
	//this->ReadDirectory(setPath, u8"PAD", &Set::LoadPads);
	//this->ReadDirectory(setPath, u8"PCM", &Set::LoadSamples);
	//this->ReadDirectory(setPath, u8"PERFORM", &Set::LoadPerformances);
	//this->LoadSongBook(setPath);
	//this->ReadDirectory(setPath, u8"SOUND", &Set::LoadSounds);
	this->ReadDirectory(setPath, u8"STYLE", &Set::LoadStyles);
}

//Private methods
void Set::LoadMultiSamples(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	ASSERT_EQUALS(1, bankEntries.GetNumberOfElements());
	this->multiSamples = dynamic_cast<MultiSample*>(bankEntries[0].object);
}

void Set::LoadPads(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	bool isUser = bankFileName.StartsWith(u8"USER");
	ASSERT(bankFileName.StartsWith(u8"BANK") || isUser, u8"???");
	ASSERT(bankFileName.EndsWith(u8".PAD"), u8"???");
	String bankPart = bankFileName.SubString(4, 2);

	uint32 bankNumber = bankPart.ToUInt32() - 1;
	if(isUser)
		bankNumber += 20;

	PadBank bank;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		Pad& pad = dynamic_cast<Pad&>(*bankObjectEntry.object);
		bank.AddObject(bankObjectEntry.name, bankObjectEntry.pos, &pad);
	}

	this->padBanks[bankNumber] = Move(bank);
}

void Set::LoadPerformances(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	ASSERT(bankFileName.StartsWith(u8"BANK"), u8"???");
	ASSERT(bankFileName.EndsWith(u8".PRF"), u8"???");
	uint8 bankNumber = bankFileName.SubString(4, 2).ToUInt32() - 1;

	ObjectBank<Performance> bank;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		Performance& performance = dynamic_cast<Performance&>(*bankObjectEntry.object);
		bank.AddObject(bankObjectEntry.name, bankObjectEntry.pos, &performance);
	}

	this->performanceBanks[bankNumber] = Move(bank);
}

void Set::LoadSamples(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries)
{
	ASSERT(bankFileName.StartsWith(u8"RAM"), u8"???");
	ASSERT(bankFileName.EndsWith(u8".PCM"), u8"???");
	uint8 bankNumber = bankFileName.SubString(3, 2).ToUInt32() - 1;

	ObjectBank<AbstractSample> bank;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		AbstractSample& sample = dynamic_cast<AbstractSample&>(*bankObjectEntry.object);
		bank.AddObject(bankObjectEntry.name, bankObjectEntry.pos, &sample);
	}

	this->sampleBanks[bankNumber] = Move(bank);
}

void Set::LoadSongs(const String& bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
		delete bankObjectEntry.object;
}

void Set::LoadSongBook(const Path& setPath)
{
	Path listsPath = setPath / String(u8"SONGBOOK/LISTDB.SBL");
	if(OSFileSystem::GetInstance().Exists(listsPath))
	{
		FileInputStream fileInputStream(listsPath);
		BankFormatReader bankFormatReader(fileInputStream);
		auto bankEntries = bankFormatReader.Read();

		for(const BankObjectEntry& bankObjectEntry : bankEntries)
			delete bankObjectEntry.object;
	}

	this->ReadDirectory(setPath, u8"SONGBOOK/SONGDB.SBD", &Set::LoadSongs);
}

void Set::LoadSounds(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	ASSERT(bankFileName.StartsWith(u8"USER"), u8"???");
	ASSERT(bankFileName.EndsWith(u8".PCG"), u8"???");
	String bankPart = bankFileName.SubString(4, 2);

	uint32 bankNumber;
	if(bankPart == u8"DK")
		bankNumber = 11;
	else
		bankNumber = 9 + bankPart.ToUInt32() - 1;

	SoundBank bank;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		Sound& sound = dynamic_cast<Sound&>(*bankObjectEntry.object);
		bank.AddObject(bankObjectEntry.name, bankObjectEntry.pos, &sound);
	}

	this->soundBanks[bankNumber] = Move(bank);
}

void Set::LoadStyles(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	Map<uint8, const BankObjectEntry*> styleEntries;
	Map<uint8, const BankObjectEntry*> performanceEntries;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		if(IS_INSTANCE_OF(bankObjectEntry.object, Style))
			styleEntries[bankObjectEntry.pos] = &bankObjectEntry;
		else
			performanceEntries[bankObjectEntry.pos] = &bankObjectEntry;
	}

	StyleBank bank;
	for(const auto& kv : styleEntries)
	{
		Style& style = dynamic_cast<Style&>(*styleEntries[kv.key]->object);
		SingleTouchSettings& sts = dynamic_cast<SingleTouchSettings&>(*performanceEntries[kv.key]->object);

		bank.AddObject(kv.value->name, kv.value->pos, new FullStyle(&style, &sts));
	}

	uint8 bankNumber = ParseStyleBankFileName(bankFileName);
	this->styleBanks[bankNumber] = Move(bank);
}

void Set::ReadDirectory(const Path &setPath, const String &dirName, void (Set::* loader)(const String& bankFileName, const DynamicArray<BankObjectEntry>&))
{
	Path dirPath = setPath / dirName;
	AutoPointer<Directory> directory = OSFileSystem::GetInstance().GetDirectory(dirPath);
	for (const String& childName : *directory)
	{
		FileInputStream fileInputStream(dirPath / childName);

		BankFormatReader korgFormatReader(fileInputStream);
		(this->*loader)(childName, korgFormatReader.Read());
	}
}
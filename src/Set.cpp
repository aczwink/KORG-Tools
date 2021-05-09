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
#include <libkorg/BankFormat/SingleTouchSettings.hpp>
#include <libkorg/Text.hpp>
#include <libkorg/BankFormat/Reader.hpp>
#include "BankFormat/Writer.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
Set::Set(const Path &setPath) : setPath(setPath)
{
	File setDir(setPath);
	ASSERT(setDir.Exists(), u8"Set does not exist");

	//TODO: GLOBAL
	this->ReadDirectory(setPath, u8"MULTISMP", &Set::LoadMultiSamples);
	//this->ReadDirectory(setPath, u8"PAD", &Set::LoadPads);
	this->ReadDirectory(setPath, u8"PCM", &Set::LoadSamples);
	this->ReadDirectory(setPath, u8"PERFORM", &Set::LoadPerformances);
	//this->LoadSongBook(setPath);
	this->ReadDirectory(setPath, u8"SOUND", &Set::LoadSounds);
	this->ReadDirectory(setPath, u8"STYLE", &Set::LoadStyles);
}

//Public methods
void Set::Save(const Model& targetModel)
{
	this->SaveBanks(this->sampleBanks, u8"PCM", targetModel);
	//this->SaveBanks(this->soundBanks, u8"SOUND");
	//this->SaveBanks(this->styleBanks, u8"STYLE");
}

//Class functions
Set Set::Create(const Path &targetPath)
{
	File dir(targetPath);
	dir.CreateDirectory();

	return Set(targetPath);
}

//Private methods
void Set::LoadMultiSamples(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	if(!bankEntries.IsEmpty())
	{
		ASSERT_EQUALS(1, bankEntries.GetNumberOfElements());
		this->multiSamples = dynamic_cast<MultiSamplesObject*>(bankEntries[0].object);
	}
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

	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		AbstractSample& sample = dynamic_cast<AbstractSample&>(*bankObjectEntry.object);
		this->sampleBanks[bankNumber].AddObject(bankObjectEntry.name, bankObjectEntry.pos, &sample);
	}
}

void Set::LoadSongs(const String& bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
		delete bankObjectEntry.object;
}

void Set::LoadSongBook(const Path& setPath)
{
	Path listsPath = setPath / String(u8"SONGBOOK/LISTDB.SBL");
	if(File(listsPath).Exists())
	{
		FileInputStream fileInputStream(listsPath);
		NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
		/*BankFormat::Reader bankFormatReader;
		bankFormatReader.ReadData(fileInputStream);
		auto bankEntries = bankFormatReader.TakeEntries();

		for(const BankObjectEntry& bankObjectEntry : bankEntries)
			delete bankObjectEntry.object;*/
	}

	this->ReadDirectory(setPath, u8"SONGBOOK/SONGDB.SBD", &Set::LoadSongs);
}

void Set::LoadSounds(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		SoundObject& sound = dynamic_cast<SoundObject&>(*bankObjectEntry.object);
		this->soundBanks[SoundBankNumber::FromBankFileName(bankFileName)].AddObject(bankObjectEntry.name, bankObjectEntry.pos, &sound);
	}
}

void Set::LoadStyles(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	BinaryTreeMap<uint8, const BankObjectEntry*> styleEntries;
	BinaryTreeMap<uint8, const BankObjectEntry*> performanceEntries;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		if(IS_INSTANCE_OF(bankObjectEntry.object, StyleObject))
			styleEntries[bankObjectEntry.pos] = &bankObjectEntry;
		else
			performanceEntries[bankObjectEntry.pos] = &bankObjectEntry;
	}

	StyleBank bank;
	for(const auto& kv : styleEntries)
	{
		StyleObject& style = dynamic_cast<StyleObject&>(*styleEntries[kv.key]->object);
		SingleTouchSettings* sts = nullptr;
		if(performanceEntries.Contains(kv.key))
			sts = dynamic_cast<SingleTouchSettings*>(performanceEntries[kv.key]->object);

		bank.AddObject(kv.value->name, kv.value->pos, new FullStyle(&style, sts));
	}
	bank.saved = true;

	uint8 bankNumber = ParseStyleBankFileName(bankFileName);
	this->styleBanks[bankNumber] = Move(bank);
}

void Set::ReadDirectory(const Path &setPath, const String &dirName, void (Set::* loader)(const String& bankFileName, const DynamicArray<BankObjectEntry>&))
{
	Path dirPath = setPath / dirName;

	File directory(dirPath);
	if(!directory.Exists())
		return;
	for (const DirectoryEntry& childEntry : directory)
	{
		FileInputStream fileInputStream(dirPath / childEntry.name);

		BankFormat::Reader bankFormatReader;
		bankFormatReader.ReadData(fileInputStream);

		(this->*loader)(childEntry.name, bankFormatReader.TakeEntries());
	}
}

template<typename BankObjectType>
void Set::SaveBank(const ObjectBank<BankObjectType>& bank, SeekableOutputStream& outputStream, const Model& targetModel)
{
	BankFormat::Writer bankFormatWriter(outputStream, targetModel);
	bankFormatWriter.Write(bank);
}

template<typename BankNumberType, typename BankObjectType>
void Set::SaveBanks(BankCollection<BankNumberType, BankObjectType>& bankCollection, const StdXX::String& folderName, const Model& targetModel)
{
	for(const auto& bankEntry : bankCollection.Entries())
	{
		if(bankEntry.bank.saved)
			continue;

		auto dirPath = this->setPath / folderName;
		auto path = dirPath / bankEntry.bankNumber.ToFileName();
		if(bankEntry.bank.Objects() >> Any())
		{
			File banksDir(dirPath);
			if(!banksDir.Exists())
				banksDir.CreateDirectory();

			StdXX::FileOutputStream fileOutputStream(path, true);
			this->SaveBank(bankEntry.bank, fileOutputStream, targetModel);
		}
		else
		{
			File bankFile(path);
			if(bankFile.Exists())
				bankFile.DeleteFile();
		}

		bankCollection[bankEntry.bankNumber].saved = true;
	}
}

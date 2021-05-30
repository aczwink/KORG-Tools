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
#include "BankFormat/ObjectBankWriter.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

static class UnknownModel : public Model
{
public:
	bool IsSampleCompressionSupported() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	BankSetup GetBankSetup() const override
	{
		return {
			.performanceBanks = {
				.factoryBankIds = {1, 16}, //PA3X
				.localBankIds = {{23, 25}}, //PA700 Oriental
			},
			.soundBanks = {
				.nUserBanks = 4, //PA4X
			},
			.styleBanks = {
				.factoryBankIds = {1, 15}, //PA600
				.userBankIds = {0, 0}, //TODO:
				.favoriteBankIds = {}, //TODO:
				.localBankIds = {{101, 105}}, //PA700 Oriental
				.nStylesPerBank = {}, //TODO:
			}
		};
	}

	String GetCustomization() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	String GetMachId() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	String GetName() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	uint32 GetSampleRAMSize() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	SupportedResourceVersions GetSupportedResourceVersions() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
} unknownModel;

//Constructors
Set::Set(const Path &setPath) : Set(setPath, unknownModel)
{
}

Set::Set(const Path &setPath, const Model& model) : setPath(setPath), model(model),
	sampleBanks(model), soundBanks(model), performanceBanks(model), styleBanks(model)
{
	File setDir(setPath);
	ASSERT(setDir.Exists(), u8"Set does not exist");

	//TODO: GLOBAL
	this->ReadDirectory(setPath, u8"MULTISMP", &Set::LoadMultiSamples);
	//this->ReadDirectory(setPath, u8"PAD", &Set::LoadPads);
	this->ReadDirectory(setPath, u8"PCM", &Set::LoadSamples);
	if(!this->ReadDirectory(setPath, u8"KEYBOARDSET", &Set::LoadPerformances))
		this->ReadDirectory(setPath, u8"PERFORM", &Set::LoadPerformances);
	//this->LoadSongBook(setPath);
	this->ReadDirectory(setPath, u8"SOUND", &Set::LoadSounds);
	this->ReadDirectory(setPath, u8"STYLE", &Set::LoadStyles);
}

//Public methods
uint32 Set::ComputeUsedSampleRAMSize()
{
	uint32 sum = 0;
	for(const auto& bankEntry : this->sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
			sum += objectEntry.object->GetSize();
	}

	return sum;
}

void Set::Save()
{
	this->SaveMultiSamples();
	this->SaveBanks(this->sampleBanks, u8"PCM");
	this->SaveBanks(this->performanceBanks, this->model.GetSupportedResourceVersions().maxPerformanceVersion.major > 1 ? u8"KEYBOARDSET" : u8"PERFORM");
	this->SaveBanks(this->soundBanks, u8"SOUND");
	this->SaveBanks(this->styleBanks, u8"STYLE");
}

//Class functions
Set Set::Create(const Path &targetPath, const Model& targetModel)
{
	File dir(targetPath);
	dir.CreateDirectory();

	return Set(targetPath, targetModel);
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
	bank.saved = true;

	this->padBanks[bankNumber] = Move(bank);
}

void Set::LoadPerformances(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	PerformanceBankNumber bankNumber = PerformanceBankNumber::FromBankFileName(bankFileName);

	ObjectBank<PerformanceObject> bank;
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		PerformanceObject& performance = dynamic_cast<PerformanceObject&>(*bankObjectEntry.object);
		bank.AddObject(bankObjectEntry.name, bankObjectEntry.pos, &performance);
	}
	bank.saved = true;

	this->performanceBanks[bankNumber] = Move(bank);
}

void Set::LoadSamples(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries)
{
	auto bankNumber = SampleBankNumber::FromBankFileName(bankFileName);
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		AbstractSample& sample = dynamic_cast<AbstractSample&>(*bankObjectEntry.object);
		this->sampleBanks[bankNumber].AddObject(bankObjectEntry.name, bankObjectEntry.pos, &sample);
	}
	this->sampleBanks[bankNumber].saved = true;
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
	SoundBankNumber bankNumber = SoundBankNumber::FromBankFileName(bankFileName);
	for(const BankObjectEntry& bankObjectEntry : bankEntries)
	{
		SoundObject& sound = dynamic_cast<SoundObject&>(*bankObjectEntry.object);
		this->soundBanks[bankNumber].AddObject(bankObjectEntry.name, bankObjectEntry.pos, &sound);
	}
	this->soundBanks[bankNumber].saved = true;
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

	auto bankNumber = StyleBankNumber::FromBankFileName(bankFileName);

	for(const auto& kv : styleEntries)
	{
		auto stsObject = performanceEntries[kv.key]->object;

		StyleObject& style = dynamic_cast<StyleObject&>(*styleEntries[kv.key]->object);
		SingleTouchSettings* sts = dynamic_cast<SingleTouchSettings*>(stsObject);

		this->styleBanks[bankNumber].AddObject(kv.value->name, kv.value->pos, new FullStyle(&style, sts));
	}
	this->styleBanks[bankNumber].saved = true;
}

bool Set::ReadDirectory(const Path &setPath, const String &dirName, void (Set::* loader)(const String& bankFileName, const DynamicArray<BankObjectEntry>&))
{
	Path dirPath = setPath / dirName;

	File directory(dirPath);
	if(!directory.Exists())
		return false;
	for (const DirectoryEntry& childEntry : directory)
	{
		FileInputStream fileInputStream(dirPath / childEntry.name);

		BankFormat::Reader bankFormatReader;
		bankFormatReader.ReadData(fileInputStream);

		(this->*loader)(childEntry.name, bankFormatReader.TakeEntries());
	}

	return true;
}

template<typename BankObjectType>
void Set::SaveBank(const ObjectBank<BankObjectType>& bank, SeekableOutputStream& outputStream)
{
	BankFormat::ObjectBankWriter bankFormatWriter(outputStream, this->model);
	bankFormatWriter.Write(bank);
}

template<typename BankNumberType, typename BankObjectType>
void Set::SaveBanks(BankCollection<BankNumberType, BankObjectType>& bankCollection, const StdXX::String& folderName)
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
			this->SaveBank(bankEntry.bank, fileOutputStream);
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

void Set::SaveMultiSamples()
{
	auto dirPath = this->setPath / String(u8"MULTISMP");
	auto path = dirPath / String(u8"RAM.KMP");

	if(this->multiSamples.IsNull())
	{
		File kmpFile(path);
		if(kmpFile.Exists())
			kmpFile.DeleteFile();

		File multiSamplesPath(dirPath);
		if(multiSamplesPath.Exists())
			multiSamplesPath.DeleteFile();
		return;
	}

	File banksDir(dirPath);
	if(!banksDir.Exists())
		banksDir.CreateDirectory();

	StdXX::FileOutputStream fileOutputStream(path, true);

	BankFormat::ObjectBankWriter bankFormatWriter(fileOutputStream, this->model);
	bankFormatWriter.Write(*this->multiSamples);
}

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
			.padBanks = {
				.factoryBankIds = {1, 20}, //PA600
				.localBankIds = {{31, 40}}, //PA4X
				.userBankIds = {21, 30}, //PA600
			},
			.performanceBanks = {
				.factoryBankIds = {1, 16}, //PA3X
				.localBankIds = {{23, 25}}, //PA700 Oriental
				.userBankIds = {{11, 17}} //PA4X
			},
			.soundBanks = {
				.nUserBanks = 4, //PA4X
			},
			.styleBanks = {
				.factoryBankIds = {1, 15}, //PA600
				.userBankIds = {18, 20}, //PA600
				.favoriteBankIds = {{21, 32}}, //PA600
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
	sampleBanks(model), soundBanks(model), performanceBanks(model), styleBanks(model), padBanks(model)
{
	File setDir(setPath);
	ASSERT(setDir.Exists(), u8"Set does not exist");

	//TODO: GLOBAL
	if(!this->ReadDirectory(setPath, u8"KEYBOARDSET", this->performanceBanks))
		this->ReadDirectory(setPath, u8"PERFORM", this->performanceBanks);

	this->LoadMultiSamples();

	this->ReadDirectory(setPath, u8"PAD", this->padBanks);
	this->ReadDirectory(setPath, u8"PCM", this->sampleBanks);
	this->ReadDirectory(setPath, u8"SOUND", this->soundBanks);
	this->ReadDirectory(setPath, u8"STYLE", this->styleBanks);

	//this->LoadSongBook(setPath);
}

//Public methods
uint32 Set::ComputeUsedSampleRAMSize()
{
	uint32 sum = 0;
	for(const auto& bankEntry : this->sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
			sum += objectEntry.Object().GetSize();
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
/*void Set::LoadMultiSamples(const String &bankFileName, const DynamicArray<BankObjectEntry> &bankEntries)
{
	if(!bankEntries.IsEmpty())
	{
		ASSERT_EQUALS(1, bankEntries.GetNumberOfElements());
		this->multiSamples = dynamic_cast<MultiSamplesObject*>(bankEntries[0].object);
	}
}
*/

/*
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
/*	}

	this->ReadDirectory(setPath, u8"SONGBOOK/SONGDB.SBD", &Set::LoadSongs);
}
*/

void Set::LoadMultiSamples()
{
	Path path = this->setPath / String(u8"MULTISMP") / String(u8"RAM.KMP");
	File file(path);
	if(!file.Exists())
		return;

	FileInputStream fileInputStream(path);
	BankFormat::Reader reader;
	reader.ReadMetadata(fileInputStream);
	auto entries = reader.TakeEntries();
	ASSERT_EQUALS(1, entries.GetNumberOfElements());

	fileInputStream.SeekTo(entries[0].dataOffset);
	BankFormat::BankObject* bankObject = reader.ReadBankObject(entries[0].headerEntry, fileInputStream);
	this->multiSamples = dynamic_cast<MultiSamplesObject*>(bankObject);
}

template<typename BankNumberType, typename BankObjectType>
bool Set::ReadDirectory(const Path& setPath, const String& dirName, BankCollection<BankNumberType, BankObjectType>& bankCollection)
{
	Path dirPath = setPath / dirName;

	File directory(dirPath);
	if(!directory.Exists())
		return false;
	for (const DirectoryEntry& childEntry : directory)
	{
		bankCollection.AddSourceBank(BankNumberType::FromBankFileName(childEntry.name), new FileInputStream(dirPath / childEntry.name));
	}

	return true;
}

template<typename BankNumberType, typename BankObjectType>
void Set::SaveBanks(BankCollection<BankNumberType, BankObjectType>& bankCollection, const StdXX::String& folderName)
{
	for(const auto& bankEntry : bankCollection.Entries())
	{
		auto dirPath = this->setPath / folderName;
		auto path = dirPath / bankEntry.bankNumber.ToFileName();

		bankCollection[bankEntry.bankNumber].Save(path);
	}
}

void Set::SaveMultiSamples()
{
	auto dirPath = this->setPath / String(u8"MULTISMP");
	auto path = dirPath / String(u8"RAM.KMP");

	if(this->multiSamples.IsNull() or this->multiSamples->IsEmpty())
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
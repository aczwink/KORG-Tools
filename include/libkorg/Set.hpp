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
#pragma once
#include <StdXX.hpp>
#include "libkorg/BankFormat/ObjectBank.hpp"
#include "libkorg/BankFormat/MultiSamplesObject.hpp"
#include <libkorg/BankFormat/SoundObject.hpp>
#include "libkorg/BankFormat/PerformanceObject.hpp"
#include "FullStyle.hpp"
#include "Model.hpp"
#include <libkorg/BankFormat/SoundBankNumber.hpp>
#include <libkorg/BankFormat/PerformanceBankNumber.hpp>
#include <libkorg/BankFormat/StyleBankNumber.hpp>
#include <libkorg/BankFormat/SampleBankNumber.hpp>
#include <libkorg/BankFormat/BankCollection.hpp>
#include <libkorg/BankFormat/PadBankNumber.hpp>

namespace libKORG
{
	struct BankObjectEntry
	{
		StdXX::String name;
		uint8 pos;
		BankFormat::BankObject* object;
	};

	class Set
	{
	public:
		//Members
		const Model& model;
		BankCollection<PadBankNumber, StyleObject> padBanks;
		BankCollection<PerformanceBankNumber, PerformanceObject> performanceBanks;
		BankCollection<SampleBankNumber, AbstractSample> sampleBanks;
		BankCollection<SoundBankNumber, SoundObject> soundBanks;
		BankCollection<StyleBankNumber, FullStyle> styleBanks;

		//Constructors
		Set(const StdXX::FileSystem::Path& setPath);
		Set(const StdXX::FileSystem::Path& setPath, const Model& model);

		//Properties
		inline MultiSamplesObject& MultiSamples()
		{
			if(this->multiSamples.IsNull())
				this->multiSamples = new MultiSamplesObject();
			return *this->multiSamples;
		}

		inline const MultiSamplesObject& MultiSamples() const
		{
			return *this->multiSamples;
		}

		//Methods
		uint32 ComputeUsedSampleRAMSize();
		void Save();

		//Functions
		static Set Create(const StdXX::FileSystem::Path& targetPath, const Model& targetModel);

		inline static ProgramChangeSequence CreateRAMSoundProgramChangeSequence(const SoundBankNumber& soundBankNumber, uint8 pos)
		{
			uint8 msb = 121 - soundBankNumber.IsDrumKit();
			uint8 lsb = 64 + soundBankNumber.UserNumber();
			return ProgramChangeSequence(msb, lsb, pos);
		}

		inline static bool IsRAMSound(const ProgramChangeSequence& programChangeSequence, const Model& model)
		{
			if(StdXX::Math::IsValueInInterval(programChangeSequence.BankSelectMSB(), (uint8)120, (uint8)121) && StdXX::Math::IsValueInInterval(programChangeSequence.BankSelectLSB(), (uint8)64, (uint8)67))
				return true;
			return false;
		}

	private:
		//Members
		StdXX::FileSystem::Path setPath;
		StdXX::UniquePointer<MultiSamplesObject> multiSamples;

		//Methods
		void LoadMultiSamples(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadPads(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadPerformances(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSamples(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSongs(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSongBook(const StdXX::FileSystem::Path& setPath);
		void LoadSounds(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadStyles(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		bool ReadDirectory(const StdXX::FileSystem::Path& setPath, const StdXX::String& dirName, void (Set::* loader)(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>&));

		template<typename BankObjectType>
		void SaveBank(const ObjectBank<BankObjectType>& bank, StdXX::SeekableOutputStream& outputStream);
		template<typename BankNumberType, typename BankObjectType>
		void SaveBanks(BankCollection<BankNumberType, BankObjectType>& bankCollection, const StdXX::String& folderName);
		void SaveMultiSamples();
	};
}
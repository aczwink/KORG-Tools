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
#include "libkorg/BankFormat/Pad.hpp"
#include "libkorg/BankFormat/Performance.hpp"
#include "FullStyle.hpp"
#include <libkorg/BankFormat/SoundBankNumber.hpp>
#include <libkorg/BankFormat/PerformanceBankNumber.hpp>
#include <libkorg/BankFormat/StyleBankNumber.hpp>
#include <libkorg/BankFormat/SampleBankNumber.hpp>

namespace libKORG
{
	typedef ObjectBank<Pad> PadBank;
	typedef ObjectBank<Performance> PerformanceBank;
	typedef ObjectBank<AbstractSample> SampleBank;
	typedef ObjectBank<SoundObject> SoundBank;
	typedef ObjectBank<FullStyle> StyleBank;

	struct BankObjectEntry
	{
		StdXX::String name;
		uint8 pos;
		BankFormat::BankObject* object;
	};

	class Set
	{
	public:
		//Constructors
		Set(const StdXX::FileSystem::Path& setPath);

		//Properties
		inline const MultiSamplesObject& MultiSamples() const
		{
			return *this->multiSamples;
		}

		inline const StdXX::Map<PerformanceBankNumber, PerformanceBank>& PerformanceBanks() const
		{
			return this->performanceBanks;
		}

		inline const StdXX::Map<SampleBankNumber, SampleBank>& SampleBanks() const
		{
			return this->sampleBanks;
		}

		inline const StdXX::Map<SoundBankNumber, SoundBank>& SoundBanks() const
		{
			return this->soundBanks;
		}

		inline StdXX::Map<StyleBankNumber, StyleBank>& StyleBanks()
		{
			return this->styleBanks;
		}

		inline const StdXX::Map<StyleBankNumber, StyleBank>& StyleBanks() const
		{
			return this->styleBanks;
		}

		//Methods
		void Save();

		//Functions
		static Set Create(const StdXX::FileSystem::Path& targetPath);

	private:
		//Members
		StdXX::FileSystem::Path setPath;
		StdXX::UniquePointer<MultiSamplesObject> multiSamples;
		StdXX::Map<uint8, PadBank> padBanks;
		StdXX::Map<PerformanceBankNumber, PerformanceBank> performanceBanks;
		StdXX::Map<SampleBankNumber, SampleBank> sampleBanks;
		StdXX::Map<SoundBankNumber, SoundBank> soundBanks;
		StdXX::Map<StyleBankNumber, StyleBank> styleBanks;

		//Methods
		void LoadMultiSamples(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadPads(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadPerformances(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSamples(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSongs(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSongBook(const StdXX::FileSystem::Path& setPath);
		void LoadSounds(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void LoadStyles(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>& bankEntries);
		void ReadDirectory(const StdXX::FileSystem::Path& setPath, const StdXX::String& dirName, void (Set::* loader)(const StdXX::String& bankFileName, const StdXX::DynamicArray<BankObjectEntry>&));
	};
}
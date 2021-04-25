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
#include "ObjectBank.hpp"
#include "MultiSample.hpp"
#include "Sound.hpp"
#include "Pad.hpp"
#include "Performance.hpp"
#include "FullStyle.hpp"
#include "SoundBankNumber.hpp"

namespace libKORG
{
	typedef ObjectBank<Pad> PadBank;
	typedef ObjectBank<Performance> PerformanceBank;
	typedef ObjectBank<AbstractSample> SampleBank;
	typedef ObjectBank<Sound> SoundBank;
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
		inline const StdXX::Map<uint8, PerformanceBank>& PerformanceBanks() const
		{
			return this->performanceBanks;
		}

		inline const StdXX::Map<uint8, SampleBank>& SampleBanks() const
		{
			return this->sampleBanks;
		}

		inline const StdXX::Map<SoundBankNumber, SoundBank>& SoundBanks() const
		{
			return this->soundBanks;
		}

		inline StdXX::Map<uint8, StyleBank>& StyleBanks()
		{
			return this->styleBanks;
		}

		inline const StdXX::Map<uint8, StyleBank>& StyleBanks() const
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
		StdXX::UniquePointer<MultiSample> multiSamples;
		StdXX::Map<uint8, PadBank> padBanks;
		StdXX::Map<uint8, PerformanceBank> performanceBanks;
		StdXX::Map<uint8, SampleBank> sampleBanks;
		StdXX::Map<SoundBankNumber, SoundBank> soundBanks;
		StdXX::Map<uint8, StyleBank> styleBanks;

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
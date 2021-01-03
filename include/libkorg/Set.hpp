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
#include "BankFormatReader.hpp"
#include "ObjectBank.hpp"
#include "MultiSample.hpp"
#include "Sound.hpp"
#include "Pad.hpp"
#include "Performance.hpp"
#include "Style.hpp"
#include "SingleTouchSettings.hpp"

namespace libKORG
{
	class FullStyle
	{
	public:
		//Constructor
		inline FullStyle(UniquePointer<class Style>&& style, UniquePointer<SingleTouchSettings>&& sts)
				: style(Move(style)), sts(Move(sts))
		{
		}

		//Properties
		inline const class Style& Style() const
		{
			return *this->style;
		}

		inline const SingleTouchSettings& STS() const
		{
			return *this->sts;
		}

	private:
		//Members
		UniquePointer<class Style> style;
		UniquePointer<SingleTouchSettings> sts;
	};

	typedef ObjectBank<Pad> PadBank;
	typedef ObjectBank<Performance> PerformanceBank;
	typedef ObjectBank<AbstractSample> SampleBank;
	typedef ObjectBank<Sound> SoundBank;
	typedef ObjectBank<FullStyle> StyleBank;

	class Set
	{
	public:
		//Members

		//Constructor
		Set(const StdXX::FileSystem::Path& setPath);

		//Properties
		inline const StdXX::Map<uint8, SampleBank>& SampleBanks() const
		{
			return this->sampleBanks;
		}

		inline const StdXX::Map<uint8, StyleBank>& StyleBanks() const
		{
			return this->styleBanks;
		}

	private:
		//Members
		StdXX::UniquePointer<MultiSample> multiSamples;
		StdXX::Map<uint8, PadBank> padBanks;
		StdXX::Map<uint8, PerformanceBank> performanceBanks;
		StdXX::Map<uint8, SampleBank> sampleBanks;
		StdXX::Map<uint8, SoundBank> soundBanks;
		StdXX::Map<uint8, StyleBank> styleBanks;

		//Methods
		void LoadMultiSamples(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void LoadPads(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void LoadPerformances(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSamples(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSongs(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void LoadSongBook(const FileSystem::Path& setPath);
		void LoadSounds(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void LoadStyles(const String& bankFileName, const DynamicArray<BankObjectEntry>& bankEntries);
		void ReadDirectory(const StdXX::FileSystem::Path& setPath, const StdXX::String& dirName, void (Set::* loader)(const String& bankFileName, const DynamicArray<BankObjectEntry>&));
	};
}
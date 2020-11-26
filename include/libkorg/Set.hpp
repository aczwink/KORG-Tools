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
#include <StdXX.hpp>
#include "BankFormatReader.hpp"
#include "ObjectBank.hpp"
#include "MultiSample.hpp"
#include "Sound.hpp"

namespace libKORG
{
	typedef ObjectBank<AbstractSample> SampleBank;
	typedef ObjectBank<Sound> SoundBank;

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

	private:
		//Members
		StdXX::Map<uint8, SampleBank> sampleBanks;
		StdXX::Map<uint8, SoundBank> soundBanks;
		StdXX::UniquePointer<MultiSample> multiSamples;

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
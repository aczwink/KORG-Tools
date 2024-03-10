/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
#include <StdXXTest.hpp>
#include <libkorg.hpp>
//Namespaces
using namespace libKORG;
using namespace libKORG::Sound;
using namespace StdXX;

TEST_SUITE(NewSoundWithOSCBasicModificationsTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/sounds/pa600/new_withoscbasicmodifications.SET");
		Set set(setPath);
		const auto soundEntry = set.soundBanks.Entries().begin().operator*().bank.Objects().begin().operator*();
		const auto& soundData = soundEntry.Object().data;

		ASSERT_EQUALS(1, soundData.oscillators.GetNumberOfElements());

		ASSERT_EQUALS(-1, soundData.oscillators[0].octaveTranspose);
		ASSERT_EQUALS(2, soundData.oscillators[0].transpose);
		ASSERT_EQUALS(1, soundData.oscillators[0].tune);
		ASSERT_EQUALS(3, soundData.oscillators[0].velocityMultiSampleSwitchLowHigh);
	}
}
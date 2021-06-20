/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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

TEST_SUITE(AllValuesSetDrumKitTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/sounds/pa600/allvaluesset_dk.SET");
		Set set(setPath);
		const auto soundEntry = set.soundBanks.Entries().begin().operator*().bank.Objects().begin().operator*();
		const auto& soundData = soundEntry.object->data;

		ASSERT_EQUALS(true, soundData.drumKitData.HasValue());
		const auto& dkData = *soundData.drumKitData;
		const auto& layer1 = dkData.layers[0];
		const auto& layer2 = dkData.layers[1];

		//TODO: ROM/RAM

		ASSERT_EQUALS(Reversed::No, layer1.reversed);
		ASSERT_EQUALS(28, layer1.drumSampleNumber);
		ASSERT_EQUALS(-99, layer1.level);
		ASSERT_EQUALS(-41, layer1.transpose);
		ASSERT_EQUALS(45, layer1.tune);
		ASSERT_EQUALS(-64, layer1.attack);
		ASSERT_EQUALS(63, layer1.decay);
		ASSERT_EQUALS(-63, layer1.cutoff);
		ASSERT_EQUALS(62, layer1.resonance);

		ASSERT_EQUALS(true, layer1.drumSampleEqualizer.enable);
		ASSERT_EQUALS(53, layer1.drumSampleEqualizer.trim);
		ASSERT_EQUALS(-18 * 2, layer1.drumSampleEqualizer.lowGainTimes2);
		//TODO: freq???
		ASSERT_EQUALS(18 * 2, layer1.drumSampleEqualizer.midGainTimes2);
		ASSERT_EQUALS(-12.5 * 2, layer1.drumSampleEqualizer.highGainTimes2);

		ASSERT_EQUALS(-99, layer2.intensity);
	}
}
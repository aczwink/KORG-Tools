/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "../EventComparison.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

TEST_SUITE(SimpleMaqsumTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/simple_maqsum.SET");
		Set set(setPath);
		const auto& fullStyle = set.styleBanks.Entries().begin().operator*().bank.Objects().begin().operator*().Object();
		const auto& styleData = fullStyle.Style().data;

		StyleView styleView(styleData);
		const auto& korfDrumTrack = styleView.GetVariation(0).GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Drum);

		FileInputStream fileInputStream(String(u8"testdata/styles/pa600/simple_maqsum.SET/V1-CV1.MID"));
		auto smfProgram = MIDI::Program::Load(fileInputStream);
		const auto& smfDrumTrack = smfProgram.GetChannelTrack(9);

		ASSERT_EQUALS(2, smfProgram.MetaTrack().GetNumberOfElements());
		ASSERT_EQUALS(30, smfDrumTrack.GetNumberOfElements());

		//standard midi will have the following at the beginning of each chord variation:
		//time signature
		// control Change bundle #00-32 (Bank Select MSB/LSB)
		// program Change
		// control Change #11 (Expression)
	}
}
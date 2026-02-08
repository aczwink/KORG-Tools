/*
 * Copyright (c) 2024-2026 Amir Czwink (amir130@hotmail.de)
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
#include "../../Shared.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

TEST_SUITE(SimpleMaqsumTests)
{
	TEST_CASE(Test)
	{
		const uint8 general_midi_percussionChannelZeroBased = 10 - 1;

		FileSystem::Path setPath(u8"testdata/styles/pa600/simple_maqsum.SET");
		Set set(setPath);
		const auto& fullStyle = set.styleBanks.Entries().begin().operator*().bank.Objects().begin().operator*().Object();
		uint8 bpm = fullStyle.STS().MetronomeTempo();
		const auto& styleData = fullStyle.Style().data;

		StyleView styleView(styleData);
		const auto& korfDrumTrack = FindSingleTrackForAccompaniment(styleView.GetVariation(0).GetChordVariation(0), AccompanimentTrackNumber::Drum);

		StandardMIDIFormatConverter converter(bpm, styleView.TimeSignature(), true);
		auto convertedSmfProgram = converter.LoadVariation(0, 0, styleData);
		const auto& convertedSmfDrumTrack = convertedSmfProgram.GetChannelTrack(general_midi_percussionChannelZeroBased);

		FileInputStream fileInputStream(String(u8"testdata/styles/pa600/simple_maqsum.SET/V1-CV1.MID"));
		auto smfProgram = MIDI::Program::Load(fileInputStream);
		const auto& smfDrumTrack = smfProgram.GetChannelTrack(general_midi_percussionChannelZeroBased);

		ASSERT_EQUALS(smfProgram.MetaTrack(), convertedSmfProgram.MetaTrack());
		ASSERT_EQUALS(smfDrumTrack, convertedSmfDrumTrack);
	}
}
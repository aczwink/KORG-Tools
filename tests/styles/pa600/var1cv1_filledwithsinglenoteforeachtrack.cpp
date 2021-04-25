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
#include "../EventComparison.hpp"
//Namespaces
using namespace StdXX;

TEST_SUITE(Var1CV1FilledWithSingleNoteForEachTrackTests)
{
	void ShouldHaveSingleNoteEvent(const DynamicArray<KORG_MIDI_Event>& events, const Pitch& expectedPitch)
	{
		RangedEventView eventRangeFinder(events);
		auto eventRanges = eventRangeFinder.Events();

		ASSERT_EQUALS(1, eventRanges.GetNumberOfElements());
		const auto& rangedEvent = eventRanges[0];

		ASSERT_EQUALS(RangedEventType::Note, rangedEvent.type);
		ASSERT_EQUALS(0, rangedEvent.position);
		ASSERT_EQUALS(expectedPitch.Encode(), rangedEvent.value1);
		ASSERT_EQUALS(100, rangedEvent.value2);
		ASSERT_EQUALS(192, rangedEvent.length);
	}

	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/var1cv1_filledwithsinglenoteforeachtrack.SET");
		Set set(setPath);
		const auto& styleEntry = set.StyleBanks().begin().operator*().value.Objects().begin().operator*().value;
		const auto& style = styleEntry.Get<1>()->Style().data;

		StyleView styleView(style);
		OnlyVar1ShouldBeEnabled(styleView);

		//Var1
		//CV1
		ShouldHaveSingleNoteEvent(style.midiTracks[0].events, Pitch(OctavePitch::C, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[1].events, Pitch(OctavePitch::C_SHARP, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[2].events, Pitch(OctavePitch::D, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[3].events, Pitch(OctavePitch::D_SHARP, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[4].events, Pitch(OctavePitch::E, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[5].events, Pitch(OctavePitch::F, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[6].events, Pitch(OctavePitch::F_SHARP, -1));
		ShouldHaveSingleNoteEvent(style.midiTracks[7].events, Pitch(OctavePitch::G, -1));
	}
}
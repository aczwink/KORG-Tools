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
using namespace StdXX;

TEST_SUITE(Var2CV1AndVar3CV1HaveSingleDrumNoteTests)
{
	void ShouldHaveSingleNoteEvent(const DynamicArray<KORG_MIDI_Event>& events, uint8 startTime, uint8 length)
	{
		RangedEventView eventRangeFinder(events);
		auto eventRanges = eventRangeFinder.Events();

		ASSERT_EQUALS(1, eventRanges.GetNumberOfElements());
		const auto& rangedEvent = eventRanges[0];

		ASSERT_EQUALS(RangedEventType::Note, rangedEvent.type);
		ASSERT_EQUALS(startTime, rangedEvent.position);
		ASSERT_EQUALS(Pitch(OctavePitch::C, 4).Encode(), rangedEvent.value1);
		ASSERT_EQUALS(100, rangedEvent.value2);
		ASSERT_EQUALS(length, rangedEvent.length);
	}

	void CheckEnabledStyleElements(const StyleView& styleView)
	{
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(0)); //var1 is always enabled
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(1));
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(2));
		ASSERT_EQUALS(false, styleView.IsVariationEnabled(3));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Break));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Ending1));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Ending2));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Ending3));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Intro1));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Intro2));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Intro3));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Fill1));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Fill2));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Fill3));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Fill4));
	}

	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/var2cv1_and_var3cv1_havesingledrumnote.SET");
		Set set(setPath);
		const auto& styleEntry = set.StyleBanks().begin().operator*().value.Objects().begin().operator*().value;
		const auto& styleData = styleEntry.Get<1>()->Style().data;
		StyleView styleView(styleData);

		CheckEnabledStyleElements(styleView);

		ShouldHaveSingleNoteEvent(styleData.midiTracks[3].events, 3, 191);
		ShouldHaveSingleNoteEvent(styleData.midiTracks[4].events, 9, 241);
	}
}
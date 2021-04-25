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

TEST_SUITE(Fill1AndFill4HaveSameBassTrackAndFill3HasDifferentBassTrack)
{
	void CheckEnabledStyleElements(const StyleView& styleView)
	{
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(0)); //var1 is always enabled
		ASSERT_EQUALS(false, styleView.IsVariationEnabled(1));
		ASSERT_EQUALS(false, styleView.IsVariationEnabled(2));
		ASSERT_EQUALS(false, styleView.IsVariationEnabled(3));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Break));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Ending1));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Ending2));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Ending3));

		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Intro1));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Intro2));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Intro3));

		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill1));
		ASSERT_EQUALS(false, styleView.IsStyleElementEnabled(StyleElementNumber::Fill2));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill3));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill4));
	}

	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/fill1_and_fill4_havesamebasstrack_and_fill3_hasdifferentbasstrack.SET");
		Set set(setPath);
		const auto& styleEntry = set.StyleBanks().begin().operator*().value.Objects().begin().operator*().value;
		const auto& styleData = styleEntry.Get<1>()->Style().data;
		StyleView styleView(styleData);

		CheckEnabledStyleElements(styleView);
	}
}
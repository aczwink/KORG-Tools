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

TEST_SUITE(EmptyStyleTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/empty.SET");
		Set set(setPath);
		const auto& styleEntry = set.StyleBanks().begin().operator*().value.Objects().begin().operator*().value;
		const auto& styleData = styleEntry.Get<1>()->Style().data;

		StyleView styleView(styleData);
		OnlyVar1ShouldBeEnabled(styleView);

		for(const auto& track : styleData.midiTracks)
			ShouldHaveNoEvents(track.events);

		for(const auto& styleElement : styleData.styleElements)
		{
			for(const auto& cv : styleElement.cv)
				ShouldHaveNoEvents(cv.midiTrack.events);
		}

		for(const auto& styleElement : styleData.variation)
		{
			for(const auto& cv : styleElement.cv)
				ShouldHaveNoEvents(cv.midiTrack.events);
		}
	}
}
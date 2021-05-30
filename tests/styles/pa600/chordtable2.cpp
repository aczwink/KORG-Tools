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
#include "../../Shared.hpp"
//Namespaces
using namespace StdXX;

TEST_SUITE(ChordTable2Tests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/chordtable2.SET");
		Set set(setPath);
		const auto& fullStyle = ExtractFirstStyle(set);
		const auto& styleData = fullStyle.Style().data;

		const auto& ct = styleData.variation[3].chordTable;
		ASSERT_EQUALS(5, ct.seven_sharp5CVIndex);
		ASSERT_EQUALS(4, ct.M7sharp5CVIndex);
		ASSERT_EQUALS(3, ct.onePlusFiveCVIndex);
		ASSERT_EQUALS(2, ct.onePlusEightCVIndex);
	}
}
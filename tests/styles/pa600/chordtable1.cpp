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

TEST_SUITE(ChordTable1Tests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/chordtable1.SET");
		Set set(setPath);
		const auto& fullStyle = ExtractFirstStyle(set);
		const auto& styleData = fullStyle.Style().data;


		const auto& ct1 = styleData.variation[0].chordTable;
		ASSERT_EQUALS(5, ct1.majorCVIndex);
		ASSERT_EQUALS(4, ct1.sixCVIndex);
		ASSERT_EQUALS(3, ct1.M7CVIndex);
		ASSERT_EQUALS(2, ct1.b5CVIndex);
		ASSERT_EQUALS(1, ct1.M7b5CVIndex);

		const auto& ct2 = styleData.variation[1].chordTable;
		ASSERT_EQUALS(1, ct2.susCVIndex);
		ASSERT_EQUALS(2, ct2.sus2CVIndex);
		ASSERT_EQUALS(3, ct2.M7susCVIndex);
		ASSERT_EQUALS(4, ct2.mCVIndex);
		ASSERT_EQUALS(5, ct2.m6CVIndex);

		const auto& ct3 = styleData.variation[2].chordTable;
		ASSERT_EQUALS(1, ct3.m7CVIndex);
		ASSERT_EQUALS(2, ct3.m7b5CVIndex);
		ASSERT_EQUALS(3, ct3.mM7CVIndex);
		ASSERT_EQUALS(4, ct3.sevenCVIndex);
		ASSERT_EQUALS(5, ct3.seven_b5CVIndex);

		const auto& ct4 = styleData.variation[3].chordTable;
		ASSERT_EQUALS(1, ct4.seven_susCVIndex);
		ASSERT_EQUALS(2, ct4.dimCVIndex);
		ASSERT_EQUALS(3, ct4.dim7CVIndex);
		ASSERT_EQUALS(4, ct4.dimM7CVIndex);
		ASSERT_EQUALS(5, ct4.sharp5CVIndex);
	}
}
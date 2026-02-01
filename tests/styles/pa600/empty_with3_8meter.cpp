/*
 * Copyright (c) 2026 Amir Czwink (amir130@hotmail.de)
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

TEST_SUITE(EmptyWith3On8Meter)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/empty_with3_8meter.SET");
		Set set(setPath);
		const auto& fullStyle = set.styleBanks.Entries().begin().operator*().bank.Objects().begin().operator*().Object();
		
		StyleView styleView(fullStyle.Style().data);
		ASSERT_EQUALS(Math::Rational<uint8>(3, 8), styleView.TimeSignature());
	}
}
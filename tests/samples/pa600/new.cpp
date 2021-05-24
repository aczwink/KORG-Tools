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

TEST_SUITE(NewSampleTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/samples/pa600/new.SET");
		Set set(setPath);
		const auto& sampleEntry = set.sampleBanks.Entries().begin().operator*().bank.Objects().begin().operator*().object;

		//TODO: CHECK SAMPLE
		//TODO: CHECK MULTISAMPLE
	}
}
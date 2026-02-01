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

TEST_SUITE(TimeSignature)
{
	void TestTimeSig(const Set& set, const StyleBankNumber& bankNumber, uint8 pos, uint8 num, uint8 den)
	{
		StyleView styleView(set.styleBanks[bankNumber].GetObject(pos).Style().data);
		ASSERT_EQUALS(Math::Rational<uint8>(num, den), styleView.TimeSignature());
	}

	TEST_CASE(Test)
	{
		StyleBankNumber bank4 = StyleBankNumber::FromBankName(u8"USER07");
		StyleBankNumber bank4_2 = StyleBankNumber::FromBankName(u8"USER10");
		StyleBankNumber bank8 = StyleBankNumber::FromBankName(u8"USER08");
		StyleBankNumber bank8_2 = StyleBankNumber::FromBankName(u8"USER11");
		StyleBankNumber bank16 = StyleBankNumber::FromBankName(u8"USER09");
		StyleBankNumber bank16_2 = StyleBankNumber::FromBankName(u8"USER12");

		FileSystem::Path setPath(u8"testdata/styles/pa700oriental/time_signatures.SET");
		Set set(setPath);

		//fourths
		TestTimeSig(set, bank4, 0, 1, 4);
		TestTimeSig(set, bank4, 1, 2, 4);
		TestTimeSig(set, bank4, 2, 3, 4);
		TestTimeSig(set, bank4, 3, 5, 4);
		TestTimeSig(set, bank4, 4, 6, 4);
		TestTimeSig(set, bank4, 5, 7, 4);
		TestTimeSig(set, bank4, 6, 8, 4);
		TestTimeSig(set, bank4, 7, 9, 4);

		//fourths second half
		TestTimeSig(set, bank4_2, 0, 4, 4);
		TestTimeSig(set, bank4_2, 1, 10, 4);
		TestTimeSig(set, bank4_2, 2, 11, 4);
		TestTimeSig(set, bank4_2, 3, 12, 4);
		TestTimeSig(set, bank4_2, 4, 13, 4);
		TestTimeSig(set, bank4_2, 5, 14, 4);
		TestTimeSig(set, bank4_2, 6, 15, 4);
		TestTimeSig(set, bank4_2, 7, 16, 4);

		//eights
		TestTimeSig(set, bank8, 0, 1, 8);
		TestTimeSig(set, bank8, 1, 2, 8);
		TestTimeSig(set, bank8, 2, 3, 8);
		TestTimeSig(set, bank8, 3, 4, 8);
		TestTimeSig(set, bank8, 4, 5, 8);
		TestTimeSig(set, bank8, 5, 6, 8);
		TestTimeSig(set, bank8, 6, 7, 8);
		TestTimeSig(set, bank8, 7, 8, 8);
		TestTimeSig(set, bank8, 8, 9, 8);
		TestTimeSig(set, bank8, 9, 10, 8);

		//eights second half
		TestTimeSig(set, bank8_2, 0, 11, 8);
		TestTimeSig(set, bank8_2, 1, 12, 8);
		TestTimeSig(set, bank8_2, 2, 13, 8);
		TestTimeSig(set, bank8_2, 3, 14, 8);
		TestTimeSig(set, bank8_2, 4, 15, 8);
		TestTimeSig(set, bank8_2, 5, 16, 8);

		//sixteenths second half
		TestTimeSig(set, bank16_2, 0, 1, 16);
		TestTimeSig(set, bank16_2, 1, 2, 16);
		TestTimeSig(set, bank16_2, 2, 3, 16);
		TestTimeSig(set, bank16_2, 3, 4, 16);
		TestTimeSig(set, bank16_2, 4, 5, 16);
		TestTimeSig(set, bank16_2, 5, 6, 16);
		TestTimeSig(set, bank16_2, 6, 7, 16);

		//sixteenths
		TestTimeSig(set, bank16, 0, 8, 16);
		TestTimeSig(set, bank16, 1, 9, 16);
		TestTimeSig(set, bank16, 2, 10, 16);
		TestTimeSig(set, bank16, 3, 11, 16);
		TestTimeSig(set, bank16, 4, 12, 16);
		TestTimeSig(set, bank16, 5, 13, 16);
		TestTimeSig(set, bank16, 6, 14, 16);
		TestTimeSig(set, bank16, 7, 15, 16);
		TestTimeSig(set, bank16, 8, 16, 16);
	}
}
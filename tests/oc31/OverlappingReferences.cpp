/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "../../src/BankFormat/OC31Compressor.hpp"
#include "../../src/BankFormat/OC31.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

TEST_SUITE(OC31Tests)
{
	TEST_CASE(OverlappingBackreferences)
	{
		const char8_t* data = u8"abcdef-abc-abcdef";

		DynamicByteBuffer buffer;
		OC31Compressor compressor(buffer.CreateOutputStream());

		compressor.WriteBytes(data, 6 + 1 + 3 + 1 + 6);

		compressor.Finalize();

		auto input = buffer.CreateInputStream();
		DynamicArray<OC31Block> blocks = OC31ReadAllBlocks(*input);

		ASSERT_EQUALS(3, blocks.GetNumberOfElements());

		const auto& b1 = blocks[0];
		ASSERT_EQUALS(false, b1.isBackreference);
		ASSERT_EQUALS(7, b1.length);

		const auto& b2 = blocks[1];
		ASSERT_EQUALS(true, b2.isBackreference);
		ASSERT_EQUALS(6, b2.distance);
		ASSERT_EQUALS(3, b2.length);
		ASSERT_EQUALS(1, b2.nExtraBytes); //here is the second dash

		//here it is cheaper to put the second dash as an extra byte and then taking the backreference "abcdef" instead of "-abc" and "def" separately
		//thus saving 1 byte
		const auto& b3 = blocks[2];
		ASSERT_EQUALS(true, b3.isBackreference);
		ASSERT_EQUALS(10, b3.distance);
		ASSERT_EQUALS(6, b3.length);
		ASSERT_EQUALS(0, b3.nExtraBytes);
	}
}
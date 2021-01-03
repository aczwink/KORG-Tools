/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#pragma once
#include <StdXX.hpp>

namespace libKORG
{
	class ProgramChangeSequence
	{
	public:
		//Constructor
		inline ProgramChangeSequence(uint8 bankSelectMSB, uint8 bankSelectLSB, uint8 programChange)
		{
			this->bankSelectMSB = bankSelectMSB;
			this->bankSelectLSB = bankSelectLSB;
			this->programChange = programChange;
		}

		//Inline
		inline StdXX::String ToString() const
		{
			return StdXX::String::Number(this->bankSelectMSB) + u8"." + this->ZeroFill(this->bankSelectLSB) + u8"." + this->ZeroFill(this->programChange);
		}

		//Functions
		static ProgramChangeSequence FromEncoded(uint32 encoded)
		{
			ASSERT_EQUALS(0, (encoded >> 8) & 0xFF);

			return ProgramChangeSequence(encoded >> 24, (encoded >> 16) & 0xFF, encoded & 0xFF);
		}

		static ProgramChangeSequence Read(StdXX::DataReader& dataReader)
		{
			uint8 msb = dataReader.ReadByte();
			uint8 lsb = dataReader.ReadByte();
			uint8 programChange = dataReader.ReadByte();

			return ProgramChangeSequence(msb, lsb, programChange);
		}

	private:
		//Members
		uint8 bankSelectMSB;
		uint8 bankSelectLSB;
		uint8 programChange;

		//Inline
		inline StdXX::String ZeroFill(uint8 value) const
		{
			StdXX::String string = StdXX::String::Number(value);
			while(string.GetLength() < 3)
				string = u8"0" + string;
			return string;
		}
	};
}
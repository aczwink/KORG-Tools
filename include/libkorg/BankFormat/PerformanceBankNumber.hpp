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
#include "BankNumber.hpp"

namespace libKORG
{
	class PerformanceBankNumber : public BankNumber
	{
	public:
		//Constructor
		inline PerformanceBankNumber(uint8 number) : BankNumber(number)
		{
		}

		//Inline
		inline StdXX::String ToFileName() const
		{
			return this->ToString() + u8".PRF";
		}

		inline StdXX::String ToString() const
		{
			StdXX::String numberAsString = StdXX::String::Number(this->Number() + 1, 10, 2);
			return u8"BANK" + numberAsString;
		}

		//Functions
		static PerformanceBankNumber FromBankName(const StdXX::String& bankName)
		{
			uint8 shift;
			StdXX::String bankPart;
			if(bankName.StartsWith(u8"LOCAL"))
			{
				bankPart = bankName.SubString(5);
				shift = 22;
			}
			else if(bankName.StartsWith(u8"USER"))
			{
				bankPart = bankName.SubString(4);
				shift = 10;
			}
			else
			{
				ASSERT(bankName.StartsWith(u8"BANK"), u8"???");
				bankPart = bankName.SubString(4);
				shift = 0;
			}

			uint8 bankNumber = bankPart.ToUInt32() + shift - 1;

			return {bankNumber};
		}

		static PerformanceBankNumber FromBankFileName(const StdXX::String& bankFileName)
		{
			ASSERT(bankFileName.EndsWith(u8".PRF"), u8"???");
			return PerformanceBankNumber::FromBankName(bankFileName.SubString(0, bankFileName.GetLength() - 4));
		}

		static PerformanceBankNumber FromId(uint8 id)
		{
			return {BankNumber::IdToNumber(id)};
		}
	};
}
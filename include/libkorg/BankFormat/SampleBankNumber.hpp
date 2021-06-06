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
	class SampleBankNumber : public BankNumber
	{
	public:
		//Constructor
		inline SampleBankNumber(uint8 number) : BankNumber(number)
		{
		}

		//Inline
		inline uint8 PresentationNumber() const
		{
			return this->Number() + 1;
		}

		inline StdXX::String ToFileName() const
		{
			return this->ToString() + u8".PCM";
		}

		inline StdXX::String ToString() const
		{
			return u8"RAM" + StdXX::String::Number(this->PresentationNumber(), 10, 2);
		}

		//Functions
		static SampleBankNumber FromBankName(const StdXX::String& bankName)
		{
			ASSERT(bankName.StartsWith(u8"RAM"), u8"???");
			uint8 bankNumber = bankName.SubString(3).ToUInt32() - 1;
			return bankNumber;
		}

		static SampleBankNumber FromBankFileName(const StdXX::String& bankFileName)
		{
			ASSERT(bankFileName.EndsWith(u8".PCM"), u8"???");
			return SampleBankNumber::FromBankName(bankFileName.SubString(0, bankFileName.GetLength() - 4));
		}

		static SampleBankNumber FromId(uint8 id)
		{
			return {BankNumber::IdToNumber(id)};
		}
	};
}
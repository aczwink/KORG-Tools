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
#pragma once
#include "BankNumber.hpp"

namespace libKORG
{
	class PadBankNumber : public BankNumber
	{
	public:
		//Constructor
		inline PadBankNumber(uint8 number) : BankNumber(number)
		{
		}

		//Inline
		inline StdXX::String ToString() const
		{
			if(this->Number() >= 20)
				return u8"USER" + StdXX::String::Number(this->Id() - 20, 10, 2);
			return u8"BANK" + StdXX::String::Number(this->Id(), 10, 2);
		}

		//Functions
		static PadBankNumber FromBankName(const StdXX::String& bankName)
		{
			bool isLocal = bankName.StartsWith(u8"LOCAL");
			bool isUser = bankName.StartsWith(u8"USER");
			ASSERT(bankName.StartsWith(u8"BANK") || isUser || isLocal, u8"???");

			StdXX::String bankPart;
			if(isLocal)
			{
				bankPart = bankName.SubString(5);
			}
			else
			{
				bankPart = bankName.SubString(4);
			}

			uint32 bankNumber = bankPart.ToUInt32() - 1;
			if(isLocal)
				bankNumber += 30;
			else if(isUser)
				bankNumber += 20;

			return {(uint8)bankNumber};
		}

		static PadBankNumber FromBankFileName(const StdXX::String& bankFileName)
		{
			ASSERT(bankFileName.EndsWith(u8".PAD"), u8"???");
			return PadBankNumber::FromBankName(bankFileName.SubString(0, bankFileName.GetLength() - 4));
		}

		static PadBankNumber FromId(uint8 id)
		{
			return {BankNumber::IdToNumber(id)};
		}
	};
}
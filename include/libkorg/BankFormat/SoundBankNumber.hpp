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
#include "BankNumber.hpp"

namespace libKORG
{
	class SoundBankNumber : public BankNumber
	{
	public:
		//Constructor
		inline SoundBankNumber(uint8 number) : BankNumber(number)
		{
			ASSERT(StdXX::Math::IsValueInInterval((int)number, 9, 13), StdXX::String::Number(number));
		}

		//Properties
		inline bool IsDrumKit() const
		{
			return this->Number() == 11;
		}

		inline uint8 UserNumber() const
		{
			if(this->IsDrumKit())
				return 0;
			switch(this->Number())
			{
				case 9:
				case 10:
					return this->Number() - 9;
				case 12:
				case 13:
					return this->Number() - 12 + 2;
			}
			RAISE(StdXX::ErrorHandling::IllegalCodePathError);
		}

		//Inline
		inline StdXX::String ToFileName() const
		{
			return this->ToString() + u8".PCG";
		}

		inline StdXX::String ToString() const
		{
			if(this->IsDrumKit())
				return u8"USERDK";
			return u8"USER" + StdXX::String::Number(this->UserNumber() + 1, 10, 2);
		}

		//Functions
		static SoundBankNumber FromBankName(const StdXX::String& bankName)
		{
			ASSERT(bankName.StartsWith(u8"USER"), u8"???");
			StdXX::String bankPart = bankName.SubString(4);

			uint8 bankNumber;
			if(bankPart == u8"DK")
				bankNumber = 11;
			else
			{
				uint32 bankPartAsNumber = bankPart.ToUInt32();
				if(bankPartAsNumber <= 2)
					bankNumber = 9 + bankPartAsNumber - 1;
				else
					bankNumber = 12 + bankPartAsNumber - 3;
			}

			return {bankNumber};
		}

		static SoundBankNumber FromBankFileName(const StdXX::String& bankFileName)
		{
			ASSERT(bankFileName.EndsWith(u8".PCG"), u8"???");
			return SoundBankNumber::FromBankName(bankFileName.SubString(0, bankFileName.GetLength() - 4));
		}
	};
}
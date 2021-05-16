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
	class SoundBankNumber
	{
	public:
		//Constructor
		inline SoundBankNumber(uint8 number) : number(number)
		{
		}

		//Properties
		inline bool IsDrumKit() const
		{
			return this->number == 11;
		}

		inline uint8 UserNumber() const
		{
			if(this->IsDrumKit())
				return 0;
			return this->number - 9;
		}

		//Operators
		inline bool operator<(const SoundBankNumber& rhs) const
		{
			return this->number < rhs.number;
		}

		inline bool operator>(const SoundBankNumber& rhs) const
		{
			return this->number > rhs.number;
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
				bankNumber = 9 + bankPart.ToUInt32() - 1;

			return {bankNumber};
		}

		static SoundBankNumber FromBankFileName(const StdXX::String& bankFileName)
		{
			ASSERT(bankFileName.EndsWith(u8".PCG"), u8"???");
			return SoundBankNumber::FromBankName(bankFileName.SubString(0, bankFileName.GetLength() - 4));
		}

	private:
		//Members
		uint8 number;
	};
}
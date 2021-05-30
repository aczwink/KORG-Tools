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
	class BankNumber
	{
	public:
		//Constructor
		inline BankNumber(uint8 number) : number(number)
		{
		}

		//Properties
		inline uint8 Id() const
		{
			return this->number + 1;
		}

		inline uint8 Number() const
		{
			return this->number;
		}

		//Operators
		inline bool operator<(const BankNumber& rhs) const
		{
			return this->number < rhs.number;
		}

		inline bool operator>(const BankNumber& rhs) const
		{
			return this->number > rhs.number;
		}

	private:
		//Members
		uint8 number;
	};
}
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
#include <StdXX.hpp>
#include "BankObject.hpp"

namespace libKORG
{
	enum class KeySignature
	{
		Unspecified = 0,

		A_Major = 17,
		Bb_Major = 18,
		B_Major = 19,
		C_Major = 20,
		CSharp_Major = 21,
		D_Major = 22,
		Eb_Major = 23,
		E_Major = 24,
		F_Major = 25,
		FSharp_Major = 26,
		G_Major = 27,
		GSharp_Major = 28,

		A_Minor = 33,
		Bb_Minor = 34,
		B_Minor = 35,
		C_Minor = 36,
		CSharp_Minor = 37,
		D_Minor = 38,
		Eb_Minor = 39,
		E_Minor = 40,
		F_Minor = 41,
		FSharp_Minor = 42,
		G_Minor = 43,
		GSharp_Minor = 44,
	};

	enum class Meter
	{
		//TODO: rest
		ThreeOnFour = 34,
		FourOnFour = 35,
		//TODO: rest
		FifteenOnSixteen = 78,
		//TODO: rest
	};

	enum class ThreeState
	{
		Unchanged = 0,
		Off = 1,
		On = 2
	};

	class SongBookEntry : public BankFormat::BankObject
	{
	public:
		//Constructor
		inline SongBookEntry(StdXX::InputStream& inputStream)
		{
			this->ReadData(inputStream);
		}

	private:
		//Methods
		void ReadData(StdXX::InputStream& inputStream);
	};
}
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
//Local
#include "libkorg/BankFormat/SingleTouchSettings.hpp"
#include "libkorg/BankFormat/Style.hpp"

namespace libKORG
{
	class FullStyle
	{
	public:
		//Constructors
		inline FullStyle(StdXX::UniquePointer<class StyleObject>&& style, StdXX::UniquePointer<SingleTouchSettings>&& sts)
				: style(Move(style)), sts(Move(sts))
		{
		}

		inline FullStyle(const FullStyle& other) : style(new class StyleObject(*other.style)), sts(new SingleTouchSettings(*other.sts))
		{
		}

		//Properties
		inline const class StyleObject& Style() const
		{
			return *this->style;
		}

		inline const SingleTouchSettings& STS() const
		{
			return *this->sts;
		}

	private:
		//Members
		StdXX::UniquePointer<class StyleObject> style;
		StdXX::UniquePointer<SingleTouchSettings> sts;
	};
}
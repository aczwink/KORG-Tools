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
#include "Performance.hpp"

namespace libKORG
{
	class SingleTouchSettings : public BankObject
	{
	public:
		//Constructor
		inline SingleTouchSettings(struct AccompanimentSettings&& accompanimentSettings,
				StdXX::StaticArray<libKORG::KeyboardSettings, 4>&& keyboardSettings)
				: accompanimentSettings(StdXX::Move(accompanimentSettings)), keyboardSettings(Move(keyboardSettings))
		{
		}

		//Properties
		inline const auto& AccompanimentSettings() const
		{
			return this->accompanimentSettings;
		}

		//Inline
		inline const auto& GetSTS(uint8 number) const
		{
			return this->keyboardSettings[number];
		}

		void WriteData(StdXX::DataWriter &dataWriter) const override
		{
			NOT_IMPLEMENTED_ERROR;
		}

	private:
		//Members
		struct AccompanimentSettings accompanimentSettings;
		StdXX::StaticArray<libKORG::KeyboardSettings, 4> keyboardSettings;
	};
}
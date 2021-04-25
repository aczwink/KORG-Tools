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
//Local
#include "libkorg/ChunkFormat/ChunkFormat.hpp"

namespace libKORG
{
	class Model
	{
	public:
		struct BankSetup
		{
			StdXX::Math::Range<uint8> factoryBankIds;
			StdXX::Math::Range<uint8> userBankIds;
			StdXX::Optional<StdXX::Math::Range<uint8>> favoriteBankIds;
			StdXX::Optional<StdXX::Math::Range<uint8>> localBankIds;
			uint8 nStylesPerBank;
		};

		//Abstract
		virtual BankSetup GetBankSetup() const = 0;
		virtual StdXX::String GetCustomization() const = 0;
		virtual StdXX::String GetMachId() const = 0;
		virtual ChunkVersion GetMaximumPerformanceVersion() const = 0;
		virtual StdXX::String GetName() const = 0;

		//Properties
		inline StdXX::String InternalName() const
		{
			return u8"Z" + this->GetMachId() + this->GetCustomization();
		}
	};

	const Model* FindModel(const StdXX::String& name);
}
/*
 * Copyright (c) 2021-2023 Amir Czwink (amir130@hotmail.de)
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
			struct
			{
				StdXX::Math::Interval<uint8> factoryBankIds;
				StdXX::Optional<StdXX::Math::Interval<uint8>> localBankIds;
				StdXX::Math::Interval<uint8> userBankIds;
			} padBanks;

			struct
			{
				StdXX::Math::Range<uint8> factoryBankIds;
				StdXX::Optional<StdXX::Math::Range<uint8>> localBankIds;
				StdXX::Optional<StdXX::Math::Range<uint8>> userBankIds;
			} performanceBanks;

			struct
			{
				uint8 nUserBanks;
			} soundBanks;

			struct
			{
				StdXX::Math::Range<uint8> factoryBankIds;
				StdXX::Math::Range<uint8> userBankIds;
				StdXX::Optional<StdXX::Math::Range<uint8>> favoriteBankIds;
				StdXX::Optional<StdXX::Math::Range<uint8>> localBankIds;
				uint8 nStylesPerBank;
			} styleBanks;
		};

		struct SupportedResourceVersions
		{
			ChunkVersion maxMultiSamplesVersion;
			ChunkVersion maxPCMVersion;
			ChunkVersion maxPerformanceVersion;
			ChunkVersion maxSoundVersion;
			ChunkVersion maxStyleVersion;
		};

		//Abstract
		virtual bool IsSampleCompressionSupported() const = 0;
		virtual BankSetup GetBankSetup() const = 0;
		virtual StdXX::String GetCustomization() const = 0;
		virtual StdXX::String GetMachId() const = 0;
		virtual StdXX::String GetName() const = 0;
		/**
		 * This means "uncompressed" samples i.e. 16-bit linear PCM.
		 * @return in MiB (official spec says MB but actually gives figures in MiB).
		 */
		virtual uint32 GetSampleRAMSize() const = 0;
		virtual SupportedResourceVersions GetSupportedResourceVersions() const = 0;

		//Properties
		inline StdXX::String InternalName() const
		{
			return u8"Z" + this->GetMachId() + this->GetCustomization();
		}

		//Operators
		inline bool operator==(const Model& rhs) const
		{
			return this == &rhs;
		}
	};

	const Model* FindModel(const StdXX::String& name);
}
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
//Local
#include "PadBankNumber.hpp"
#include "BankSlot.hpp"

namespace libKORG
{
	template <typename BankNumberType, typename ObjectType>
	class BankCollection
	{
		struct BankEntry
		{
			const BankNumberType& bankNumber;
			const ObjectBank<ObjectType>& bank;

			inline BankEntry(const BankNumberType& bankNumber, const ObjectBank<ObjectType>& bank) : bankNumber(bankNumber), bank(bank)
			{
			}

			BankEntry(const BankEntry& rhs) = default;
			BankEntry(BankEntry&& rhs) = default;

			BankEntry& operator=(const BankEntry&) = delete;
			BankEntry& operator=(BankEntry&&) = delete;
		};
	public:
		//Constructor
		inline BankCollection(const Model& model) : model(model)
		{
		}

		//Operators
		inline ObjectBank<ObjectType>& operator[](BankNumberType number) const
		{
			ASSERT_EQUALS(true, this->HasBank(number));
			if(!this->banks.Contains(number))
				this->banks.Insert(number, ObjectBank<ObjectType>(this->model));
			return this->banks.Get(number);
		}

		//Methods
		StdXX::Optional<BankSlot<BankNumberType>> FindFreeSlot(BankNumberType startBankNumber = 0, uint8 startPos = 0) const
		{
			uint8 bankNumber = startBankNumber.Number();
			uint8 pos = startPos;
			do
			{
				if(this->HasBank(bankNumber))
				{
					auto result = this->operator[](bankNumber).FindFreeSlot(pos);
					if(result.HasValue())
						return {{bankNumber, *result}};
				}

				bankNumber = (bankNumber + 1) % 99; //sample banks are the largest ones with 99 banks
				pos = 0;
			}
			while( !( (bankNumber == startBankNumber.Number()) and (pos == startPos) ) );

			return {};
		}

		//Inline
		inline auto Entries() const
		{
			return this->banks.Entries() >> StdXX::Map<const StdXX::KeyValuePair<BankNumberType, ObjectBank<ObjectType>>&, BankEntry>([](const auto& kv)
			{
				return BankEntry(kv.key, kv.value);
			});
		}

		inline StdXX::Optional<BankSlot<BankNumberType>> FindFreeSlot(const BankSlot<BankNumberType>& startSlot) const
		{
			return this->FindFreeSlot(startSlot.bankNumber, startSlot.pos);
		}

		inline bool HasBank(const BankNumberType& bankNumberType) const
		{
			return this->template HasBankImpl(bankNumberType);
		}

	private:
		//Members
		const Model& model;
		mutable StdXX::BinaryTreeMap<BankNumberType, ObjectBank<ObjectType>> banks;

		//Inline
		template<typename T = BankNumberType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, PadBankNumber>::value, bool>
			HasBankImpl(const BankNumberType& bankNumber) const
		{
			const auto padBanks = this->model.GetBankSetup().padBanks;
			if(padBanks.factoryBankIds.ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(padBanks.localBankIds.HasValue() and padBanks.localBankIds->ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(padBanks.userBankIds.ContainsEndInclusive(bankNumber.Id()))
				return true;
			return false;
		}

		template<typename T = BankNumberType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, PerformanceBankNumber>::value, bool>
		        HasBankImpl(const BankNumberType& bankNumber) const
		{
			const auto performanceBanks = this->model.GetBankSetup().performanceBanks;
			if(performanceBanks.factoryBankIds.ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(performanceBanks.localBankIds.HasValue() and performanceBanks.localBankIds->ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(performanceBanks.userBankIds.HasValue() and performanceBanks.userBankIds->ContainsEndInclusive(bankNumber.Id()))
				return true;
			return false;
		}

		template<typename T = BankNumberType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, SampleBankNumber>::value, bool>
		        HasBankImpl(const BankNumberType& bankNumber) const
		{
			return StdXX::Math::Range<int32>(1, 99).ContainsEndInclusive(bankNumber.Id());
		}

		template<typename T = BankNumberType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, SoundBankNumber>::value, bool>
		        HasBankImpl(const BankNumberType& bankNumber) const
		{
			return bankNumber.UserNumber() < this->model.GetBankSetup().soundBanks.nUserBanks;
		}

		template<typename T = BankNumberType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, StyleBankNumber>::value, bool>
		HasBankImpl(const BankNumberType& bankNumber) const
		{
			const auto styleBanks = this->model.GetBankSetup().styleBanks;
			if(styleBanks.factoryBankIds.ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(styleBanks.userBankIds.ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(styleBanks.favoriteBankIds.HasValue() and styleBanks.favoriteBankIds->ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(styleBanks.localBankIds.HasValue() and styleBanks.localBankIds->ContainsEndInclusive(bankNumber.Id()))
				return true;
			return false;
		}
	};
}
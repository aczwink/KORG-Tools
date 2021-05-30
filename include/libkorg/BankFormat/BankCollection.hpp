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
		inline ObjectBank<ObjectType>& operator[](BankNumberType number)
		{
			ASSERT_EQUALS(true, this->HasBank(number));
			return this->banks[number];
		}

		inline const ObjectBank<ObjectType>& operator[](BankNumberType number) const
		{
			ASSERT_EQUALS(true, this->HasBank(number));
			return this->banks[number];
		}

		//Inline
		inline auto Entries() const
		{
			return this->banks.Entries() >> StdXX::Map<const StdXX::KeyValuePair<BankNumberType, ObjectBank<ObjectType>>&, BankEntry>([](const auto& kv)
			{
				return BankEntry(kv.key, kv.value);
			});
		}

		inline bool HasBank(const BankNumberType& bankNumberType) const
		{
			return this->template HasBankImpl(bankNumberType);
		}

	private:
		//Members
		const Model& model;
		StdXX::BinaryTreeMap<BankNumberType, ObjectBank<ObjectType>> banks;

		//Inline
		template<typename T = BankNumberType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, PerformanceBankNumber>::value, bool>
		        HasBankImpl(const BankNumberType& bankNumber) const
		{
			const auto performanceBanks = this->model.GetBankSetup().performanceBanks;
			if(performanceBanks.factoryBankIds.ContainsEndInclusive(bankNumber.Id()))
				return true;
			if(performanceBanks.localBankIds.HasValue() and performanceBanks.localBankIds->ContainsEndInclusive(bankNumber.Id()))
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
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
		//Operators
		inline ObjectBank<ObjectType>& operator[](BankNumberType number)
		{
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

	private:
		//Members
		StdXX::BinaryTreeMap<BankNumberType, ObjectBank<ObjectType>> banks;
	};
}
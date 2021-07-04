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
#include "libkorg/BankFormat/BankObject.hpp"
#include "PerformanceObject.hpp"
#include "SoundObject.hpp"
#include <libkorg/Model.hpp>
#include <libkorg/FullStyle.hpp>

namespace libKORG
{
	template<typename ObjectType>
	class ObjectBank
	{
		struct BankObjectEntry
		{
			StdXX::String name;
			StdXX::SharedPointer<ObjectType> object;
		};

		struct ObjectEntry : public BankObjectEntry
		{
			uint8 pos;

			inline ObjectEntry(uint8 pos, const BankObjectEntry &bankObjectEntry) : pos(pos)
			{
				this->name = bankObjectEntry.name;
				this->object = bankObjectEntry.object;
			}

			ObjectEntry(const ObjectEntry &) = default;

			ObjectEntry(ObjectEntry &&) = default;

			ObjectEntry &operator=(const ObjectEntry &) = delete;

			ObjectEntry &operator=(ObjectEntry &&) = delete;
		};

	public:
		//Members
		bool saved;

		//Constructors
		inline ObjectBank(const Model &model) : model(model)
		{
			this->saved = true;
		}

		inline ObjectBank(ObjectBank&& bank) : model(bank.model)
		{
			this->objects = StdXX::Move(bank.objects);
			this->saved = bank.saved;
		}

		//Operators
		inline BankObjectEntry &operator[](uint8 pos)
		{
			return this->objects.Get(pos);
		}

		inline const BankObjectEntry &operator[](uint8 pos) const
		{
			return this->objects.Get(pos);
		}

		inline ObjectBank& operator=(ObjectBank&& bank)
		{
			ASSERT(this->model == bank.model, u8"Can't assign banks on sets for different models")

			this->objects = StdXX::Move(bank.objects);
			this->saved = bank.saved;
			return *this;
		}

		//Properties
		inline uint8 NumberOfSlots() const
		{
			return this->GetNumberOfSlots();
		}

		inline auto Objects() const
		{
			return this->objects.Entries()
					>> StdXX::Map<const StdXX::KeyValuePair<uint8, BankObjectEntry> &, ObjectEntry>([](const auto &kv) {
						return ObjectEntry(kv.key, kv.value);
					});
		}

		//Methods
		StdXX::Optional<uint8> FindFreeSlot(uint8 startPos = 0) const
		{
			uint8 pos = startPos;
			do {
				if (!this->HasObject(pos))
					return pos;
				pos = (pos + 1) % this->NumberOfSlots();
			} while (pos != startPos);

			return {};
		}

		//Inline
		inline void RemoveObject(uint8 pos)
		{
			this->objects.Remove(pos);
			this->saved = false;
		}

		inline void SetObject(const StdXX::String &name, uint8 pos, const StdXX::SharedPointer<ObjectType> &object)
		{
			this->objects[pos] = {name, object};
			this->saved = false;
		}

		inline const auto &GetName(uint8 pos) const
		{
			return this->objects.Get(pos).name;
		}

		inline bool HasObject(uint8 pos) const
		{
			return this->objects.Contains(pos);
		}

	private:
		//Members
		StdXX::BinaryTreeMap<uint8, BankObjectEntry> objects;
		const Model &model;

		//Inline
		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, AbstractSample>::value, uint8>
		GetNumberOfSlots() const
		{
			return 128; //could theoretically be 256 but apparently the bank position is signed
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, PerformanceObject>::value, uint8>
		GetNumberOfSlots() const
		{
			return 5 * 8; //5 pages and 8 performances per page
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, SoundObject>::value, uint8>
		GetNumberOfSlots() const
		{
			return 16 * 8; //16 pages and 8 sounds per page
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, FullStyle>::value, uint8>
		GetNumberOfSlots() const
		{
			return this->model.GetBankSetup().styleBanks.nStylesPerBank;
		}
	};
}
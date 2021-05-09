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

template <typename ObjectType>
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

		inline ObjectEntry(uint8 pos, const BankObjectEntry& bankObjectEntry) : pos(pos)
		{
			this->name = bankObjectEntry.name;
			this->object = bankObjectEntry.object;
		}

		ObjectEntry(const ObjectEntry&) = default;
		ObjectEntry(ObjectEntry&&) = default;

		ObjectEntry& operator=(const ObjectEntry&) = delete;
		ObjectEntry& operator=(ObjectEntry&&) = delete;
	};
public:
	//Members
	bool saved;

	//Constructor
	inline ObjectBank()
	{
		this->saved = true;
	}

	//Properties
	inline auto Objects() const
	{
		return this->objects.Entries() >> StdXX::Map<const StdXX::KeyValuePair<uint8, BankObjectEntry>&, ObjectEntry>([](const auto& kv)
		{
			return ObjectEntry(kv.key, kv.value);
		});
	}

	//Inline
	inline void AddObject(const StdXX::String& name, uint8 pos, const StdXX::SharedPointer<ObjectType>& object)
	{
		this->objects[pos] = { name, object };
		this->saved = false;
	}

private:
	//Members
	StdXX::BinaryTreeMap<uint8, BankObjectEntry> objects;
};
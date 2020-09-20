/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;
//Local
#include "StyleBankObject.hpp"

class StyleBank
{
public:
	//Properties
	inline const LinkedList<SharedPointer<StyleBankObject>>& Objects() const
	{
		return this->objects;
	}

	//Inline
	inline void AddObject(uint8 pos, const SharedPointer<StyleBankObject>& object)
	{
		this->positions.Insert(object.operator->(), pos);
		this->objects.InsertTail(object);
	}

	inline uint8 GetPositionOf(const StyleBankObject& styleBankObject) const
	{
		return this->positions.Get(&styleBankObject);
	}

private:
	//Members
	LinkedList<SharedPointer<StyleBankObject>> objects;
	Map<const StyleBankObject*, uint8> positions;
};
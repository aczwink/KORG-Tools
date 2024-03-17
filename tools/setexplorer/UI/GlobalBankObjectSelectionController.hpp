/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
#include "BankCollectionController.hpp"

template <typename BankNumberType, typename ObjectType>
class GlobalBankObjectSelectionController : public TypedBankCollectionController<BankNumberType, ObjectType>
{
public:
	//Constructor
	inline GlobalBankObjectSelectionController(SetController& setController, const BankCollection<BankNumberType, ObjectType>& bankCollection) : TypedBankCollectionController<BankNumberType, ObjectType>(bankCollection), setController(setController)
	{
	}

	//Methods
	void ObjectSelectionChanged(const BankNumberType& bankNumber, uint8 pos) override
	{
		this->TypedObjectSelectionChanged(bankNumber, pos);
	}

private:
	//State
	SetController& setController;

	//Inline
	inline void TypedObjectSelectionChanged(const PadBankNumber& bankNumber, uint8 pos)
	{
	}

	inline void TypedObjectSelectionChanged(const PerformanceBankNumber& bankNumber, uint8 pos)
	{
		this->setController.SelectPerformance(bankNumber, pos);
	}

	inline void TypedObjectSelectionChanged(const SampleBankNumber& bankNumber, uint8 pos)
	{
		this->setController.PlaySample(bankNumber, pos);
	}

	inline void TypedObjectSelectionChanged(const StyleBankNumber& bankNumber, uint8 pos)
	{
		this->setController.SelectStyle(bankNumber, pos);
	}
};
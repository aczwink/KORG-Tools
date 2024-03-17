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
#pragma once
#include "BankObjectsController.hpp"
#include "BanksController.hpp"

class BankCollectionController
{
public:
	//Abstract
	virtual void BankSelectionChanged(StdXX::Optional<uint32> row) = 0;
	virtual StdXX::SharedPointer<StdXX::UI::ListController> CreateBanksController() = 0;
	virtual StdXX::SharedPointer<StdXX::UI::ListController> CreateObjectsController() = 0;
	virtual void ObjectSelectionChanged(StdXX::Optional<uint32> row) = 0;
};

template <typename BankNumberType, typename ObjectType>
class TypedBankCollectionController : public BankCollectionController
{
public:
	//Constructor
	inline TypedBankCollectionController(const BankCollection<BankNumberType, ObjectType>& bankCollection) : bankCollection(bankCollection)
	{
		this->bankNumbers = bankCollection.GetAllBankNumbers();
	}

	//Abstract
	virtual void ObjectSelectionChanged(const BankNumberType& bankNumber, uint8 pos) = 0;

	//Methods
	void BankSelectionChanged(StdXX::Optional<uint32> row) override
	{
		if(row.HasValue())
			this->selectedBank = this->MapSelectedIndexToBankNumber(row.Value());
		else
			this->selectedBank = {};
	}

	SharedPointer<ListController> CreateBanksController() override
	{
		return new BanksController<BankNumberType, ObjectType>(this->bankNumbers);
	}

	SharedPointer<ListController> CreateObjectsController() override
	{
		if(this->selectedBank.HasValue())
			return new BankObjectsController(this->bankCollection[*this->selectedBank]);
		return nullptr;
	}

	void ObjectSelectionChanged(StdXX::Optional<uint32> row) override
	{
		if(row.HasValue())
			this->ObjectSelectionChanged(*this->selectedBank, *row);
	}

private:
	//State
	const BankCollection<BankNumberType, ObjectType>& bankCollection;
	LinkedList<BankNumberType> bankNumbers;
	StdXX::Optional<BankNumberType> selectedBank;

	//Inline
	inline BankNumberType MapSelectedIndexToBankNumber(uint32 index) const
	{
		return this->bankNumbers[index];
	}
};
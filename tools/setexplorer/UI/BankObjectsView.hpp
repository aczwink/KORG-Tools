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
#include "BankObjectsController.hpp"
#include "../SetController.hpp"
#include "BankCollectionController.hpp"

class BankObjectsView : public ListView
{
public:
    //Constructor
    inline BankObjectsView(BankCollectionController& controller) : controller(controller)
    {
        this->selectionChanged.Connect(this, &BankObjectsView::OnSelectedBankObjectChanged);
    }

private:
    //Members
	BankCollectionController& controller;

    //Private methods
    void OnSelectedBankObjectChanged()
	{
		auto indexes = this->SelectionController().GetSelectedIndexes();
		if(indexes.IsEmpty())
			this->controller.ObjectSelectionChanged({});
		else
			this->controller.ObjectSelectionChanged(indexes.GetFront().GetRow());
	}
};
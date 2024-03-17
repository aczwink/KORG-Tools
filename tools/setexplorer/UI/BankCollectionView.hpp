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
#include "BankCollectionController.hpp"
#include "BankObjectsView.hpp"

class BankCollectionView : public CompositeWidget
{
public:
    //Constructor
    BankCollectionView(BankCollectionController& controller) : controller(controller)
    {
        this->bankListView = new ListView;
        this->bankListView->selectionChanged.Connect(this, &BankCollectionView::OnSelectedBankChanged);

        this->bankListView->SetController(controller.CreateBanksController());
        this->AddChild(this->bankListView);

        this->bankObjectsView = new BankObjectsView(controller);
        this->AddChild(this->bankObjectsView);
    }

private:
    //Members
    BankCollectionController& controller;
    ListView* bankListView;
    BankObjectsView* bankObjectsView;

    //Event handlers
    void OnSelectedBankChanged()
    {
        const auto& selection = this->bankListView->SelectionController().GetSelectedIndexes();
        if(selection.IsEmpty())
            this->controller.BankSelectionChanged({});
        else
            this->controller.BankSelectionChanged(selection[0].GetRow());

        this->bankObjectsView->SetController(this->controller.CreateObjectsController());
    }
};
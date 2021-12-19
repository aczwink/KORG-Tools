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
#include "BanksController.hpp"
#include "BankObjectsController.hpp"
#include "BankObjectsView.hpp"

template <typename BankNumberType, typename ObjectType>
class BankCollectionView : public CompositeWidget
{
public:
    //Constructor
    BankCollectionView(SetController& setController, const BankCollection<BankNumberType, ObjectType>& bankCollection) : bankCollection(bankCollection)
    {
        this->bankListView = new ListView;
        this->bankListView->selectionChanged.Connect(this, &BankCollectionView::OnSelectedBankChanged);

        this->banksController = new BanksController(bankCollection);
        this->bankListView->SetController(this->banksController);
        this->AddChild(this->bankListView);

        this->bankObjectsView = new BankObjectsView<BankNumberType, ObjectType>(setController);
        this->AddChild(this->bankObjectsView);
    }

private:
    //Members
    const BankCollection<BankNumberType, ObjectType>& bankCollection;
    ListView* bankListView;
    SharedPointer<BanksController<BankNumberType, ObjectType>> banksController;
    BankObjectsView<BankNumberType, ObjectType>* bankObjectsView;

    //Event handlers
    void OnSelectedBankChanged()
    {
        const auto& selection = this->bankListView->SelectionController().GetSelectedIndexes();
        if(selection.IsEmpty())
            this->bankObjectsView->SetController(nullptr);
        else
        {
            BankNumberType bankNumber = this->banksController->MapSelectedIndexToBankNumber(selection[0].GetRow());
            this->bankObjectsView->ShowBank(bankNumber, this->bankCollection[bankNumber]);
        }
    }
};
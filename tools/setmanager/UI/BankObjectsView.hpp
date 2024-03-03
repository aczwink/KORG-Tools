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

template <typename BankNumberType, typename ObjectType>
class BankObjectsView : public ListView
{
public:
    //Constructor
    inline BankObjectsView(class SetController& setController) : setController(setController)
    {
        this->selectionChanged.Connect(this, &BankObjectsView::OnSelectedBankObjectChanged);
    }

    //Inline
    inline void ShowBank(const BankNumberType& bankNumber, const ObjectBank<ObjectType>& objectBank)
    {
        this->bankNumber = bankNumber;
        this->SetController(new BankObjectsController(objectBank));
    }

private:
    //Members
    class SetController& setController;
    Optional<BankNumberType> bankNumber;

    //Inline
    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, PadBankNumber>::value, String>
    BankNumberTypeToString() const
    {
        return u8"pad";
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, PerformanceBankNumber>::value, String>
    BankNumberTypeToString() const
    {
        return u8"performance";
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, SampleBankNumber>::value, String>
    BankNumberTypeToString() const
    {
        return u8"sample";
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, SoundBankNumber>::value, String>
    BankNumberTypeToString() const
    {
        return u8"sound";
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, StyleBankNumber>::value, String>
    BankNumberTypeToString() const
    {
        return u8"style";
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, PadBankNumber>::value, void>
    OnSelectedBankObjectChanged()
    {
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, PerformanceBankNumber>::value, void>
    OnSelectedBankObjectChanged()
    {
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, SampleBankNumber>::value, void>
    OnSelectedBankObjectChanged()
    {
		auto indexes = this->SelectionController().GetSelectedIndexes();
		if(indexes.IsEmpty())
			return;
		this->setController.PlaySample(*this->bankNumber, indexes.GetFront().GetRow());
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, SoundBankNumber>::value, void>
    OnSelectedBankObjectChanged()
    {
        auto indexes = this->SelectionController().GetSelectedIndexes();
        if(indexes.IsEmpty())
            return;
        this->setController.SelectSound(*this->bankNumber, indexes.GetFront().GetRow());
    }

    template<typename T = BankNumberType>
    inline Type::EnableIf_t< Type::IsSameType<T, StyleBankNumber>::value, void>
    OnSelectedBankObjectChanged()
    {
    }

    //Event handlers
    void OnCopyBankObject(uint8 pos)
    {
        Clipboard clipboard;

        CommonFileFormats::JsonValue data = CommonFileFormats::JsonValue::Object();

        data[u8"setPath"] = setController.Set().SetPath().String();
        data[u8"type"] = this->BankNumberTypeToString();
        data[u8"bankNumber"] = this->bankNumber->Number();
        data[u8"pos"] = pos;

        clipboard.Store(data.Dump());
    }

    void OnMouseButtonPressed(MouseClickEvent &event) override
    {
        auto indexes = this->SelectionController().GetSelectedIndexes();
        if(indexes.IsEmpty())
            return;

        else if(event.GetMouseButton() == MouseButton::Right)
        {
            Menu* menu = new Menu(u8"Perske");

            if(indexes.GetNumberOfElements() == 1)
            {
                uint8 selectedPos = indexes[0].GetRow();
                menu->AppendEntry(new Action(u8"Copy", [this, selectedPos]()
                    {
                        this->OnCopyBankObject(selectedPos);
                    })
                );
            }

            menu->ShowPopup();

            event.Accept();
        }
    }
};
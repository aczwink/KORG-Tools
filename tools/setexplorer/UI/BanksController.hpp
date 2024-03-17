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
#include <libkorg.hpp>
#include <StdXX.hpp>
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::UI;

template <typename BankNumberType, typename ObjectType>
class BanksController : public ListController
{
public:
    //Constructor
    inline BanksController(const LinkedList<BankNumberType>& bankNumbers) : bankNumbers(bankNumbers)
    {
    }

    //Methods
    uint32 GetNumberOfItems() const override
    {
        return this->bankNumbers.GetNumberOfElements();
    }

    String GetText(uint32 index) const override
    {
        return this->bankNumbers[index].ToString();
    }

private:
    //Members
    LinkedList<BankNumberType> bankNumbers;
};
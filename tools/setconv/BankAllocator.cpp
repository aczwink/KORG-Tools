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
//Class header
#include "BankAllocator.hpp"

//Public methods
void BankAllocator::Compute(const ResourceSelection &selection)
{
	this->LayoutPerformances(selection.performances);
}

//Private methods
void BankAllocator::LayoutPerformances(const BinaryTreeSet<const PerformanceObject *> &performances)
{
	//first all that can be mapped directly to a position
	for(const auto& bankEntry : this->sourceSet.performanceBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(performances.Contains(objectEntry.object.operator->()))
			{
				if(this->targetSet.performanceBanks.HasBank(bankEntry.bankNumber))
				{
					this->PlacePerformance(objectEntry.object.operator->(), bankEntry.bankNumber, objectEntry.pos, objectEntry.name);
				}
			}
		}
	}

	//map remaining performances
	PerformanceBankNumber bankNumber = 0;
	uint8 pos = 0;
	for(const auto& bankEntry : this->sourceSet.performanceBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(performances.Contains(objectEntry.object.operator->()) and !this->bankAllocation.performanceAllocation.Contains(objectEntry.object.operator->()))
			{
				while(this->occupiedPerformancePositions.Contains({bankNumber, pos}))
				{
					pos++;
					if(pos == 40)
					{
						pos = 0;
						bankNumber = bankNumber.Number() + 1;
					}
				}

				this->PlacePerformance(objectEntry.object.operator->(), bankNumber, pos, objectEntry.name);
			}
		}
	}
}
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
//Local
#include "ResourceSelector.hpp"

struct BankAllocation
{
	BinaryTreeMap<const PerformanceObject*, Tuple<PerformanceBankNumber, uint8, String>> performanceAllocation;
};

class BankAllocator
{
public:
	//Constructor
	inline BankAllocator(const Set& sourceSet, const Set& targetSet) : sourceSet(sourceSet), targetSet(targetSet)
	{
	}

	//Properties
	inline const BankAllocation& Allocation() const
	{
		return this->bankAllocation;
	}

	//Methods
	void Compute(const ResourceSelection &selection);

private:
	//Members
	const Set& sourceSet;
	const Set& targetSet;
	BankAllocation bankAllocation;
	BinaryTreeSet<Tuple<PerformanceBankNumber, uint8>> occupiedPerformancePositions;

	//Methods
	void LayoutPerformances(const BinaryTreeSet<const PerformanceObject*>& performances);

	//Inline
	inline void PlacePerformance(const PerformanceObject* performanceObject, const PerformanceBankNumber& performanceBankNumber, uint8 pos, const String& name)
	{
		this->bankAllocation.performanceAllocation.Insert(performanceObject, {performanceBankNumber, pos, name});
		this->occupiedPerformancePositions.Insert({performanceBankNumber, pos});
	}
};
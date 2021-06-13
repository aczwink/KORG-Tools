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
	BinaryTreeMap<ProgramChangeSequence, Tuple<SoundBankNumber, uint8, String>> soundAllocation;
	ProgramChangeSequence freeSoundSpot;
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
	BinaryTreeSet<Tuple<SoundBankNumber, uint8>> occupiedSoundsPositions;

	//Methods
	void LayoutPerformances(const BinaryTreeSet<const PerformanceObject*>& performances);
	void LayoutSounds(const BinaryTreeSet<ProgramChangeSequence>& sounds, bool requireFreeSoundSlot);

	//Inline
	inline void PlacePerformance(const PerformanceObject* performanceObject, const PerformanceBankNumber& performanceBankNumber, uint8 pos, const String& name)
	{
		this->bankAllocation.performanceAllocation.Insert(performanceObject, {performanceBankNumber, pos, name});
		this->occupiedPerformancePositions.Insert({performanceBankNumber, pos});
	}

	inline void PlaceSound(const ProgramChangeSequence& programChangeSequence, const SoundBankNumber& bankNumber, uint8 pos, const String& name)
	{
		this->bankAllocation.soundAllocation.Insert(programChangeSequence, {bankNumber, pos, name});
		this->occupiedSoundsPositions.Insert({bankNumber, pos});
	}
};
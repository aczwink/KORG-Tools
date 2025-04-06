/*
 * Copyright (c) 2021-2025 Amir Czwink (amir130@hotmail.de)
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
	this->LayoutSounds(selection.sounds, selection.requireFreeSoundSlot);
}

//Private methods
void BankAllocator::LayoutPerformances(const BinaryTreeSet<const PerformanceObject *> &performances)
{
	//first all that can be mapped directly to a position
	for(const auto& bankEntry : this->sourceSet.performanceBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(performances.Contains(&objectEntry.Object()))
			{
				if(this->targetSet.performanceBanks.HasBank(bankEntry.bankNumber))
				{
					this->PlacePerformance(&objectEntry.Object(), bankEntry.bankNumber, objectEntry.pos, objectEntry.Name());
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
			if(performances.Contains(&objectEntry.Object()) and !this->bankAllocation.performanceAllocation.Contains(&objectEntry.Object()))
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

				this->PlacePerformance(&objectEntry.Object(), bankNumber, pos, objectEntry.Name());
			}
		}
	}
}

void BankAllocator::LayoutSounds(const BinaryTreeSet<ProgramChangeSequence> &sounds, bool requireFreeSoundSlot)
{
	//first all that can be mapped directly to a position
	for(const auto& bankEntry : this->sourceSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			ProgramChangeSequence programChangeSequence = Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos);
			if(sounds.Contains(programChangeSequence))
			{
				if(this->targetSet.soundBanks.HasBank(bankEntry.bankNumber))
				{
					this->PlaceSound(programChangeSequence, bankEntry.bankNumber, objectEntry.pos, objectEntry.Name());
				}
			}
		}
	}

	//map remaining sounds
	SoundBankNumber bankNumber = 9;
	uint8 pos = 0;
	for(const auto& bankEntry : this->sourceSet.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			ProgramChangeSequence programChangeSequence = Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos);
			if(sounds.Contains(programChangeSequence) and !this->bankAllocation.soundAllocation.Contains(programChangeSequence))
			{
				while(this->occupiedSoundsPositions.Contains({bankNumber, pos}))
				{
					pos++;
					if(pos == 16*8)
					{
						pos = 0;
						bankNumber = bankNumber.Number() + 1;
					}
				}

				this->PlaceSound(programChangeSequence, bankNumber, pos, objectEntry.Name());
			}
		}
	}

	if(requireFreeSoundSlot)
		this->bankAllocation.freeSoundSpot = Set::CreateRAMSoundProgramChangeSequence(bankNumber, pos);
}

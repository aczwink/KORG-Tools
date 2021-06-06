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
#include <libkorg/DataViews/SetIndex.hpp>
//Local
#include <libkorg/BankFormat/SampleObject.hpp>
//Namespaces
using namespace libKORG;

//Constructor
SetIndex::SetIndex(const Set& set)
{
	for(const auto& bankEntry : set.sampleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const AbstractSample& sample = *objectEntry.object;

			this->sampleLocations.Insert(sample.GetId(), {bankEntry.bankNumber, objectEntry.pos});
		}
	}

	for(const auto& bankEntry : set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			this->soundLocations.Insert(Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos), {bankEntry.bankNumber, objectEntry.pos});
		}
	}
}

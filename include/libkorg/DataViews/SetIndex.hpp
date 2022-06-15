/*
 * Copyright (c) 2021-2022 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <libkorg/Set.hpp>

namespace libKORG
{
	class SetIndex
	{
	public:
		//Constructor
		SetIndex(const Set& set);

		//Properties
		inline BankSlot<SampleBankNumber> GetSampleLocation(uint64 id) const
		{
			return this->sampleLocations.Get(id);
		}

		inline StdXX::Optional<BankSlot<SoundBankNumber>> GetSoundLocation(const ProgramChangeSequence& programChangeSequence) const
		{
			if(this->soundLocations.Contains(programChangeSequence))
				return this->soundLocations.Get(programChangeSequence);
			return {};
		}

	private:
		//Members
		StdXX::BinaryTreeMap<uint64, BankSlot<SampleBankNumber>> sampleLocations;
		StdXX::BinaryTreeMap<ProgramChangeSequence, BankSlot<SoundBankNumber>> soundLocations;
	};
}
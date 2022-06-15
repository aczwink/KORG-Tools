/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
using namespace libKORG;
using namespace StdXX;

class PlayBackSet
{
public:
    //Constructor
    inline PlayBackSet(const Set& set) : set(set), multiSamplesIndex(set.MultiSamples().data), setIndex(set)
    {
    }

    //Inline
    inline const MultiSamples::DrumSampleEntry& GetDrumSample(uint64 id) const
    {
        return this->multiSamplesIndex.GetDrumSampleEntryById(id);
    }

    inline const AbstractSample& GetSample(uint64 id) const
    {
        auto slot = this->setIndex.GetSampleLocation(id);
        return this->set.sampleBanks[slot.bankNumber].GetObject(slot.pos);
    }

    inline const MultiSamples::SampleEntry& GetSampleEntryByIndex(uint16 index) const
    {
        return this->set.MultiSamples().data.sampleEntries[index];
    }

    inline const Sound::SoundData& GetSound(const SoundBankNumber& soundBankNumber, uint8 pos) const
    {
        return this->set.soundBanks[soundBankNumber].GetObject(pos).data;
    }

private:
    //Variables
    const Set& set;
    MultiSamplesIndex multiSamplesIndex;
    SetIndex setIndex;
};
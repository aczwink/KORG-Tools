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
//Local
#include "PlayBackSet.hpp"
#include "PlayBackFactory.hpp"

class PlayBackDrumKit
{
public:
    //Constructor
    inline PlayBackDrumKit(const Sound::DrumKitSoundData& drumKitSoundData, const PlayBackSet& playBackSet, PlayBackFactory& playBackFactory)
        : drumKitSoundData(drumKitSoundData), playBackSet(playBackSet), playBackFactory(playBackFactory)
    {
    }

    //Methods
    void PlayNote(uint8 note, uint8 velocity);
    void StopNote(uint8 note, uint8 velocity);

private:
    //Variables
    const Sound::DrumKitSoundData& drumKitSoundData;
    const PlayBackSet& playBackSet;
    PlayBackFactory& playBackFactory;
    UniquePointer<Audio::Source> keySources[128];
};
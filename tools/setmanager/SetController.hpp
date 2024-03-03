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
#pragma once
#include <libkorg.hpp>
#include "PlayBack/PlaybackManager.hpp"
using namespace libKORG;

struct TrackSelection
{
    SoundBankNumber bankNumber;
    uint8 pos;
};

class SetController
{
public:
    //State
    UI::Signal<> soundSelectionChanged;

    //Constructor
    inline SetController(class Set& set, PlaybackManager& playbackManager) : set(set), playbackManager(playbackManager)
    {
    }

    //Properties
    inline const class Set& Set() const
    {
        return this->set;
    }

    //Inline
    inline bool CanKeyBePlayed(const Pitch& pitch)
    {
        return this->playbackManager.CanKeySound(pitch);
    }

    inline void KeyDown(const Pitch& pitch, uint8 velocity)
    {
        this->playbackManager.KeyDown(pitch, velocity);
    }

    inline void KeyUp(const Pitch& pitch)
    {
        this->playbackManager.KeyUp(pitch);
    }

    inline const Optional<TrackSelection>& GetAccompanimentTrackSelection(AccompanimentTrackNumber trackNumber) const
    {
        return this->accompanimentTracks[(uint8)trackNumber];
    }

    inline const Optional<TrackSelection>& GetRealTimeTrackSelection(KeyboardTrackNumber trackNumber) const
    {
        return this->realTimeTracks[(uint8)trackNumber];
    }

    inline void PlaySample(const SampleBankNumber& bankNumber, uint8 pos)
    {
        const auto& bank = this->set.sampleBanks[bankNumber];
        if(!bank.HasObject(pos))
            return;

        uint64 id = bank.GetObject(pos).GetId();
        this->playbackManager.PlaySample(id);
    }

    inline void SelectSound(const SoundBankNumber& bankNumber, uint8 pos)
    {
        const auto& bank = this->set.soundBanks[bankNumber];
        if(!bank.HasObject(pos))
            return;

        this->realTimeTracks[(uint8)KeyboardTrackNumber::Upper1] = {bankNumber, pos};

        this->playbackManager.SelectSound(bank.GetObject(pos).data);
        this->soundSelectionChanged.Emit();
    }

private:
    //Members
    class Set& set;
    PlaybackManager& playbackManager;
    Optional<TrackSelection> accompanimentTracks[8];
    Optional<TrackSelection> realTimeTracks[4];
};
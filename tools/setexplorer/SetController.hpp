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

struct StyleSelection
{
	StyleBankNumber bankNumber;
	uint8 pos;
};

class SetController
{
public:
    //State
    UI::Signal<> soundSelectionChanged;

    //Constructor
    inline SetController(class Set& set, PlaybackManager& playbackManager) : set(set), playbackManager(playbackManager), setIndex(set)
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

    inline const Optional<TrackSelection>& GetAccompanimentTrackSelection(AccompanimentTrackNumber trackNumber) const
    {
        return this->accompanimentTracks[(uint8)trackNumber];
    }

    inline const Optional<TrackSelection>& GetRealTimeTrackSelection(KeyboardTrackNumber trackNumber) const
    {
        return this->realTimeTracks[(uint8)trackNumber];
    }

    inline bool IsTrackMuted(KeyboardTrackNumber trackNumber)
    {
        return this->playbackManager.IsTrackMuted(trackNumber);
    }

    inline void KeyDown(const Pitch& pitch, uint8 velocity)
    {
        this->playbackManager.KeyDown(pitch, velocity);
    }

    inline void KeyUp(const Pitch& pitch)
    {
        this->playbackManager.KeyUp(pitch);
    }

    inline void MuteToggleTrack(KeyboardTrackNumber trackNumber)
    {
        this->playbackManager.MuteToggleTrack(trackNumber);
    }

    inline void PlaySample(const SampleBankNumber& bankNumber, uint8 pos)
    {
        const auto& bank = this->set.sampleBanks[bankNumber];
        if(!bank.HasObject(pos))
            return;

        uint64 id = bank.GetObject(pos).GetId();
        this->playbackManager.PlaySample(id);
    }

    inline void SelectPerformance(const PerformanceBankNumber& bankNumber, uint8 pos)
    {
        const auto& bank = this->set.performanceBanks[bankNumber];
        if(!bank.HasObject(pos))
            return;

        const auto& object = bank.GetObject(pos);
        switch(object.Version())
        {
            case 0:
                for(uint8 i = 0; i < 4; i++)
                    this->SelectSound((KeyboardTrackNumber)i, object.V0Data().keyboardSettings.trackSettings[i].soundProgramChangeSeq);
                for(uint8 i = 0; i < 8; i++)
                    this->SelectSound((AccompanimentTrackNumber)i, object.V0Data().accompanimentSettings.trackSettings[i].soundProgramChangeSeq);
                break;
            case 1:
            case 2:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
        }
    }

    inline void SelectSound(AccompanimentTrackNumber accompanimentTrackNumber, const SoundBankNumber& bankNumber, uint8 pos)
    {
        const auto& bank = this->set.soundBanks[bankNumber];
        if(!bank.HasObject(pos))
            return;

        this->accompanimentTracks[(uint8)accompanimentTrackNumber] = {bankNumber, pos};

        this->playbackManager.SelectSound(accompanimentTrackNumber, bank.GetObject(pos).data);
        this->soundSelectionChanged.Emit();
    }

    inline void SelectSound(KeyboardTrackNumber keyboardTrackNumber, const SoundBankNumber& bankNumber, uint8 pos)
    {
        const auto& bank = this->set.soundBanks[bankNumber];
        if(!bank.HasObject(pos))
            return;

        this->realTimeTracks[(uint8)keyboardTrackNumber] = {bankNumber, pos};

        this->playbackManager.SelectSound(keyboardTrackNumber, bank.GetObject(pos).data);
        this->soundSelectionChanged.Emit();
    }

    template <typename TrackNumberType>
    inline void SelectSound(TrackNumberType trackNumber, const ProgramChangeSequence& programChangeSequence)
    {
        auto location = this->setIndex.GetSoundLocation(programChangeSequence);
        if(location.HasValue())
            this->SelectSound(trackNumber, location.Value().bankNumber, location.Value().pos);
    }

    inline uint8 SelectSTS(uint8 stsNumber)
	{
        const auto& object = this->set.styleBanks[this->styleSelection.Value().bankNumber].GetObject(this->styleSelection.Value().pos);
        switch(object.STS().Version())
        {
            case 0:
                for(uint8 i = 0; i < 4; i++)
                    this->SelectSound((KeyboardTrackNumber)i, object.STS().V0Data().keyboardSettings[stsNumber].trackSettings[i].soundProgramChangeSeq);
                for(uint8 i = 0; i < 8; i++)
                    this->SelectSound((AccompanimentTrackNumber)i, object.STS().V0Data().accompanimentSettings.trackSettings[i].soundProgramChangeSeq);

                return 100; //TODO: unknown metronome speed for version 0
            case 1:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
                return object.STS().V1Data()._0x04000108_data.metronomeTempo;
            case 2:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
                return object.STS().V2Data()._0x04020008_data.metronomeTempo;
        }
	}

	inline void SelectStyle(const StyleBankNumber& bankNumber, uint8 pos)
	{
		const auto& bank = this->set.styleBanks[bankNumber];
		if(!bank.HasObject(pos))
			return;

		this->styleSelection = {bankNumber, pos};
		uint8 speed = this->SelectSTS(0);

		const auto& object = bank.GetObject(pos);
		this->playbackManager.LoadStyle(object.Style().data, speed);
	}

	inline void ToggleStartStopStyle()
    {
        this->playbackManager.ToggleStartStopStyle();
    }

private:
    //Members
    class Set& set;
    SetIndex setIndex;
    PlaybackManager& playbackManager;
    Optional<TrackSelection> accompanimentTracks[8];
    Optional<TrackSelection> realTimeTracks[4];
    Optional<StyleSelection> styleSelection;
};
/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include "PlayBackView.hpp"

//Constructor
PlayBackView::PlayBackView(SetController& setController)
{
    this->SetTitle(u8"Playback");

    CompositeWidget* row = new CompositeWidget;

    GroupBox* accompany = new GroupBox;
    accompany->SetTitle(u8"Accompaniment tracks");
    row->AddChild(accompany);

    for(uint8 i = 0; i < 8; i++)
    {
        this->accompanimentTracks[i] = new TrackSettingsView((AccompanimentTrackNumber)i, setController);
        accompany->AddContentChild(this->accompanimentTracks[i]);
    }

    GroupBox* realTime = new GroupBox;
    realTime->SetTitle(u8"Real-time tracks");
    row->AddChild(realTime);

    for(uint8 i = 0; i < 4; i++)
    {
        this->realTimeTracks[i] = new TrackSettingsView((KeyboardTrackNumber)i, setController);
        realTime->AddContentChild(this->realTimeTracks[i]);
    }

    this->AddContentChild(row);

    this->startStopButton = new PushButton;
    this->startStopButton->onActivatedHandler = [&setController](){
        setController.ToggleStartStopStyle();
    };
    this->startStopButton->SetText(u8"Start");
    this->AddContentChild(this->startStopButton);
}
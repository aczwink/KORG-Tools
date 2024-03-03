/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "TrackView.hpp"

//Private methods
void TrackView::Init()
{
	this->soundNameLabel = new Label;
	this->AddContentChild(this->soundNameLabel);

	this->setController.soundSelectionChanged.Connect(this, &TrackView::UpdateState);
}

void TrackView::UpdateState()
{
	const auto& trackSel = this->isAccompanimentTrack ? this->setController.GetAccompanimentTrackSelection(this->accompanimentTrackNumber) : this->setController.GetRealTimeTrackSelection(this->keyboardTrackNumber);
	if(trackSel.HasValue())
	{
		const String& name = this->setController.Set().soundBanks[trackSel.Value().bankNumber].GetName(trackSel.Value().pos);
		this->soundNameLabel->SetText(name);
	}
	else
	{
		this->soundNameLabel->SetText(u8"-");
	}
}

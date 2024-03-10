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
#include "TrackSettingsView.hpp"

//Private methods
void TrackSettingsView::Init()
{
	this->GetContentContainer()->SetLayout(new HorizontalLayout);

	this->soundNameLabel = new Label;
	this->AddContentChild(this->soundNameLabel);

	this->muted = new CheckBox;
	this->muted->SetHint(u8"Muted");
	this->muted->toggled.Connect(this, &TrackSettingsView::OnMuteTrackToggled);
	this->AddContentChild(this->muted);

	this->setController.soundSelectionChanged.Connect(this, &TrackSettingsView::UpdateState);
	this->UpdateState();
}

void TrackSettingsView::UpdateState()
{
	const auto& trackSel = this->isAccompanimentTrack ? this->setController.GetAccompanimentTrackSelection(this->accompanimentTrackNumber) : this->setController.GetRealTimeTrackSelection(this->keyboardTrackNumber);
	if(trackSel.HasValue())
	{
		const String& name = this->setController.Set().soundBanks[trackSel.Value().bankNumber].GetName(trackSel.Value().pos);
		this->soundNameLabel->SetText(name);

		this->muted->SetEnabled(true);

		if(this->isAccompanimentTrack)
		{
			//TODO
		}
		else
		{
			this->muted->Checked(this->setController.IsTrackMuted(this->keyboardTrackNumber));
		}
	}
	else
	{
		this->soundNameLabel->SetText(u8"-");
		this->muted->SetEnabled(false);
	}
}

//Event handlers
void TrackSettingsView::OnMuteTrackToggled()
{
	if(this->isAccompanimentTrack)
	{
		//TODO
	}
	else
		this->setController.MuteToggleTrack(this->keyboardTrackNumber);
	this->UpdateState();
}

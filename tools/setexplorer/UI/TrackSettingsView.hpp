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
#include <libkorg.hpp>
#include <StdXX.hpp>
using namespace libKORG;
using namespace StdXX::UI;
#include "../SetController.hpp"

class TrackSettingsView : public GroupBox
{
public:
	//Constructors
	inline TrackSettingsView(AccompanimentTrackNumber trackNumber, SetController& setController) : setController(setController)
	{
		this->isAccompanimentTrack = true;
		this->accompanimentTrackNumber = trackNumber;
		this->SetTitle(AccompanimentTrackNumberToAbbreviatedString(trackNumber));

		this->Init();
	}

	inline TrackSettingsView(KeyboardTrackNumber trackNumber, SetController& setController) : setController(setController)
	{
		this->isAccompanimentTrack = false;
		this->keyboardTrackNumber = trackNumber;
		this->SetTitle(KeyboardTrackNumberToAbbreviatedString(trackNumber));

		this->Init();
	}

private:
	//State
	bool isAccompanimentTrack;
	AccompanimentTrackNumber accompanimentTrackNumber;
	KeyboardTrackNumber keyboardTrackNumber;
	SetController& setController;

	Label* soundNameLabel;
	CheckBox* muted;

	//Methods
	void Init();
	void UpdateState();

	//Event handlers
	void OnMuteTrackToggled();
};
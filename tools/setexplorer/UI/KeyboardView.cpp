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
#include "KeyboardView.hpp"

const uint8 g_nWhiteKeysPerOctave = 7;

//Constructor
KeyboardView::KeyboardView(SetController &setController) : setController(setController)
{
	this->sizingPolicy.SetHorizontalPolicy(UI::SizingPolicy::Policy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(UI::SizingPolicy::Policy::Expanding);

	const uint8 nKeys = sizeof(this->keys)/sizeof(this->keys[0]);
	for(uint8 key = 0; key < nKeys; key++)
	{
		auto& entry = this->keys[key];
		entry.pitch = Pitch(key);
		entry.isBlack = this->IsBlackKey(entry.pitch.pitchWithinOctave);
		entry.isSelectable = false;
	}

	this->setController.soundSelectionChanged.Connect(this, &KeyboardView::UpdateKeyStates);
}

//Private methods
void KeyboardView::DrawBlackKey(UI::Painter& painter, const KeyPitchEntry& entry, bool isSelected)
{
	if(!entry.isSelectable)
		painter.SetFillColor({0.1, 0.1, 0.1, 1});
	else if(isSelected)
		painter.SetFillColor({0.25, 0.25, 0.25, 1});
	else
		painter.SetFillColor({0, 0, 0, 1});

	painter.Rectangle(entry.rect);
	painter.Fill();
}

void KeyboardView::DrawWhiteKey(UI::Painter& painter, const KeyPitchEntry& entry, bool isSelected)
{
	if(!entry.isSelectable)
		painter.SetFillColor({0.25, 0.25, 0.25, 1});
	else if(isSelected)
		painter.SetFillColor({0.75, 0.75, 0.75, 1});
	else
		painter.SetFillColor({1, 1, 1, 1});
	painter.SetStrokeColor({0, 0, 0, 1});
	painter.SetStrokeWidth(1);

	painter.Rectangle(entry.rect);
	painter.Fill();
	painter.Stroke();

	if(entry.pitch.pitchWithinOctave == OctavePitch::C)
	{
		auto size = painter.ComputeTextSize(entry.pitch.ToString());

		Math::PointD point = entry.rect.origin;
		point.x += entry.rect.width() / 2;
		point.x -= size.width / 2;
		point.y += entry.rect.height() * 0.15;
		painter.DrawText(point, entry.pitch.ToString());
	}
}

int8 KeyboardView::FindMouseHoverKey() const
{
	const uint8 nKeys = sizeof(this->keys)/sizeof(this->keys[0]);

	int8 selectedKey = -1;
	for(uint8 key = 0; key < nKeys; key++)
	{
		const auto& entry = this->keys[key];
		if(entry.rect.Contains(this->mousePos.x, this->mousePos.y))
		{
			if(selectedKey == -1)
				selectedKey = key;
			else if(!entry.isBlack and (this->keys[selectedKey].isBlack))
				continue;
			else
				selectedKey = key;
		}
	}

	return selectedKey;
}

uint8 KeyboardView::GetOctaveRelativeKeyPos(OctavePitch octavePitch) const
{
	switch(octavePitch)
	{
		case OctavePitch::C:
		case OctavePitch::C_SHARP:
			return 0;
		case OctavePitch::D:
		case OctavePitch::D_SHARP:
			return 1;
		case OctavePitch::E:
			return 2;
		case OctavePitch::F:
		case OctavePitch::F_SHARP:
			return 3;
		case OctavePitch::G:
		case OctavePitch::G_SHARP:
			return 4;
		case OctavePitch::A:
		case OctavePitch::A_SHARP:
			return 5;
		case OctavePitch::B:
			return 6;
	}
	return 0;
}

bool KeyboardView::IsBlackKey(OctavePitch octavePitch) const
{
	switch(octavePitch)
	{
		case OctavePitch::C_SHARP:
		case OctavePitch::D_SHARP:
		case OctavePitch::F_SHARP:
		case OctavePitch::G_SHARP:
		case OctavePitch::A_SHARP:
			return true;
	}
	return false;
}

void KeyboardView::UpdateKeyStates()
{
	const uint8 nKeys = sizeof(this->keys)/sizeof(this->keys[0]);
	for(uint8 key = 0; key < nKeys; key++)
	{
		auto& entry = this->keys[key];
		entry.isSelectable = this->setController.CanKeyBePlayed(entry.pitch);
	}
	this->Repaint();
}

//Event handlers
void KeyboardView::OnMouseButtonPressed(UI::MouseClickEvent &event)
{
	int8 selectedKey = this->FindMouseHoverKey();
	if(selectedKey != -1)
	{
		this->setController.KeyDown(this->keys[selectedKey].pitch, 100);
	}
}

void KeyboardView::OnMouseButtonReleased(UI::MouseClickEvent &event)
{
	int8 selectedKey = this->FindMouseHoverKey();
	if(selectedKey != -1)
	{
		this->setController.KeyUp(this->keys[selectedKey].pitch);
	}
}

void KeyboardView::OnMouseMoved(UI::MouseEvent &event)
{
	this->mousePos = event.GetPosition();
	this->Repaint();
}

void KeyboardView::OnPaint(UI::PaintEvent& event)
{
	auto painter = this->CreatePainter(event.osHandle);

	const uint8 nKeys = sizeof(this->keys)/sizeof(this->keys[0]);

	int8 selectedKey = this->FindMouseHoverKey();
	for(uint8 key = 0; key < nKeys; key++)
	{
		const auto& entry = this->keys[key];
		if(!entry.isBlack)
			this->DrawWhiteKey(*painter, entry, selectedKey == key);
	}
	for(uint8 key = 0; key < nKeys; key++)
	{
		const auto& entry = this->keys[key];
		if(entry.isBlack)
			this->DrawBlackKey(*painter, entry, selectedKey == key);
	}

	event.Accept();
}

void KeyboardView::OnResized()
{
	const uint8 nKeys = sizeof(this->keys)/sizeof(this->keys[0]);
	const uint8 nWhiteKeys = 10 * 7 + 5; //C-1 - G9

	const float64 whiteKeyWidth = this->GetSize().width / nWhiteKeys;
	const float64 blackKeyWidth = whiteKeyWidth * 0.5;

	for(uint8 pitch = 0; pitch < nKeys; pitch++)
	{
		auto& entry = this->keys[pitch];

		const uint8 absOctave = static_cast<const uint8>(entry.pitch.octave + 1_u8);
		const uint8 key = this->GetOctaveRelativeKeyPos(entry.pitch.pitchWithinOctave);

		if(entry.isBlack)
		{
			const float64 x = ((absOctave * g_nWhiteKeysPerOctave) + key) * whiteKeyWidth + blackKeyWidth * 1.5;
			entry.rect = {x, this->GetSize().height * 0.25, blackKeyWidth, this->GetSize().height * 0.75};
		}
		else
		{
			const float64 x = ((absOctave * g_nWhiteKeysPerOctave) + key) * whiteKeyWidth;
			entry.rect = {x, 0, whiteKeyWidth, this->GetSize().height};
		}
	}
}
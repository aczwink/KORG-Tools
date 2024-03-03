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
#include "../SetController.hpp"

class KeyboardView : public UI::PathRenderTargetWidget
{
	struct KeyPitchEntry
	{
		bool isBlack;
		bool isSelectable;
		Pitch pitch;
		Math::RectD rect;
	};
public:
	//Constructor
	KeyboardView(SetController& setController);

private:
	//State
	Math::PointD mousePos;
	SetController& setController;
	KeyPitchEntry keys[128]; //C-1 to G9

	//Methods
	void DrawBlackKey(const KeyPitchEntry& entry, bool isSelected);
	void DrawWhiteKey(const KeyPitchEntry& entry, bool isSelected);
	int8 FindMouseHoverKey() const;
	uint8 GetOctaveRelativeKeyPos(OctavePitch octavePitch) const;
	bool IsBlackKey(OctavePitch octavePitch) const;
	void UpdateKeyStates();

	//Event handlers
	void OnMouseButtonPressed(UI::MouseClickEvent &event) override;
	void OnMouseButtonReleased(UI::MouseClickEvent &event) override;
	void OnMouseMoved(UI::MouseEvent &event) override;
	void OnPaint(UI::PaintEvent &event) override;
	void OnResized() override;
};
/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "ChordVariationView.hpp"

template<uint8 nCV>
class StyleElementView : public libKORG::IStyleElementView
{
public:
	//Constructor
	inline StyleElementView(const libKORG::Style::StyleData &styleData, const libKORG::Style::ChordVariationData* cv, uint8 trackBaseIndex)
	{
		for(uint8 i = 0; i < nCV; i++)
			this->cvViews[i] = new ChordVariationView(styleData, cv[i], trackBaseIndex);
	}

	const libKORG::IChordVariationView &GetChordVariation(uint8 index) const override
	{
		return *this->cvViews[index];
	}

	uint8 GetNumberOfChordVariations() const override
	{
		return nCV;
	}

private:
	//Members
	StdXX::StaticArray<StdXX::UniquePointer<libKORG::IChordVariationView>, nCV> cvViews;
};
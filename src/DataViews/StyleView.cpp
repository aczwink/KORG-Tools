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
//Class header
#include <libkorg/DataViews/StyleView.hpp>
//Local
#include "StyleElementView.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Style;

//Private methods
void StyleView::GenerateStyleElementViews(const StyleData &styleData)
{
	uint8 trackIndex = 0;

	for(uint8 i = 0; i < 4; i++)
	{
		this->variationViews[i] = new StyleElementView<6>(styleData, styleData.variation[i].cv, trackIndex);
		for(const auto & cv : styleData.variation[i].cv)
			trackIndex += cv.trackMapping.GetNumberOfElements();
	}

	for(uint8 i = 0; i < 11; i++)
	{
		this->styleElementViews[i] = new StyleElementView<2>(styleData, styleData.styleElements[i].cv, trackIndex);
		for(const auto & cv : styleData.styleElements[i].cv)
			trackIndex += cv.trackMapping.GetNumberOfElements();
	}
}

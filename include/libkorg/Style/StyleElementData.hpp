/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "StyleTrackData.hpp"
#include "ChordVariation.hpp"
#include "StyleElementInfoData.hpp"

namespace libKORG::Style
{
	struct GeneralStyleElementData
	{
		StyleElementInfoData styleElementInfoData;
		StyleTrackData styleTrackData[8];

		//Constructor
		GeneralStyleElementData() = default;

		//Operators
		inline void CopyFrom(const GeneralStyleElementData& styleElementData)
		{
			this->styleElementInfoData = styleElementData.styleElementInfoData;
			for(uint8 i = 0; i < 8; i++)
				this->styleTrackData[i] = styleElementData.styleTrackData[i];
		}
	};

	struct VariationStyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[6];

		//Constructor
		inline VariationStyleElementData()
		{
			this->styleElementInfoData.cueMode = CueMode::EnterOnNextMeasure_ContinueWithCurrentMeasure;
		}

		//Operators
		inline VariationStyleElementData& operator=(const VariationStyleElementData& styleElementData)
		{
			this->CopyFrom(styleElementData);
			for(uint8 i = 0; i < 6; i++)
				this->cv[i] = styleElementData.cv[i];
			return *this;
		}
	};

	struct StyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[2];

		//Constructor
		StyleElementData() = default;

		//Operators
		inline StyleElementData& operator=(const StyleElementData& styleElementData)
		{
			this->CopyFrom(styleElementData);
			for(uint8 i = 0; i < 2; i++)
				this->cv[i] = styleElementData.cv[i];
			return *this;
		}
	};
}
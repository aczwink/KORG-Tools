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

namespace libKORG::Style
{
	struct ChordTable
	{
		uint8 unknown1;
		uint8 unknown2;

		uint8 majorCVIndex;
		uint8 sixCVIndex;
		uint8 M7CVIndex;
		uint8 M7b5CVIndex;
		uint8 susCVIndex;
		uint8 sus2CVIndex;
		uint8 M7susCVIndex;
		uint8 mCVIndex;
		uint8 m6CVIndex;
		uint8 m7CVIndex;
		uint8 m7b5CVIndex;
		uint8 mM7CVIndex;
		uint8 sevenCVIndex;
		uint8 seven_b5CVIndex;
		uint8 seven_susCVIndex;
		uint8 dimCVIndex;
		uint8 dimM7CVIndex;
		uint8 sharp5CVIndex;
		uint8 seven_sharp5CVIndex;
		uint8 M7sharp5CVIndex;
		uint8 onePlusFiveCVIndex;
		byte onePlusEightCVIndex;
		uint8 b5CVIndex;
		uint8 dim7CVIndex;
		byte unknown3;

		StdXX::DynamicByteBuffer unknown;
	};

	struct GeneralStyleElementData
	{
		ChordTable chordTable;
		StyleTrackData styleTrackData[8];
		StdXX::DynamicByteBuffer unknown;

		//Constructors
		GeneralStyleElementData() = default;

		//Operators
		inline void CopyFrom(const GeneralStyleElementData& styleElementData)
		{
			this->chordTable = styleElementData.chordTable;
			for(uint8 i = 0; i < 8; i++)
				this->styleTrackData[i] = styleElementData.styleTrackData[i];
			this->unknown = styleElementData.unknown;
		}
	};

	struct VariationStyleElementData : public GeneralStyleElementData
	{
		ChordVariationData cv[6];

		//Constructors
		VariationStyleElementData() = default;

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
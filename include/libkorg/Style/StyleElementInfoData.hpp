/*
 * Copyright (c) 2020-2026 Amir Czwink (amir130@hotmail.de)
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

namespace libKORG::Style
{
	struct ChordTable
	{
		uint8 majorCVIndex = 0;
		uint8 sixCVIndex = 0;
		uint8 M7CVIndex = 0;
		uint8 M7b5CVIndex = 0;
		uint8 susCVIndex = 0;
		uint8 sus2CVIndex = 0;
		uint8 M7susCVIndex = 0;
		uint8 mCVIndex = 0;
		uint8 m6CVIndex = 0;
		uint8 m7CVIndex = 0;
		uint8 m7b5CVIndex = 0;
		uint8 mM7CVIndex = 0;
		uint8 sevenCVIndex = 0;
		uint8 seven_b5CVIndex = 0;
		uint8 seven_susCVIndex = 0;
		uint8 dimCVIndex = 0;
		uint8 dimM7CVIndex = 0;
		uint8 sharp5CVIndex = 0;
		uint8 seven_sharp5CVIndex = 0;
		uint8 M7sharp5CVIndex = 0;
		uint8 onePlusFiveCVIndex = 0;
		byte onePlusEightCVIndex = 0;
		uint8 b5CVIndex = 0;
		uint8 dim7CVIndex = 0;
	};

	enum class CueMode
	{
		EnterImmediately_ContinueWithFirstMeasure = 0,
		EnterImmediately_ContinueWithCurrentMeasure = 1,
		EnterOnNextMeasure_ContinueWithFirstMeasure = 2,
		EnterOnNextMeasure_ContinueWithCurrentMeasure = 3,
	};

	struct StyleElementInfoData
	{
		uint8 chordVariationsWithData;
		uint8 encodedTimeSignature;
		ChordTable chordTable;
		CueMode cueMode;
		uint8 unknown4[3] = {0, 0, 0};
		uint8 unknown5[12] = {};
		uint8 unknown6 = 1;

		//Constructor
		StyleElementInfoData() = default;

		//Properties
		inline uint8 TimeSignatureDenominator() const
		{
			return static_cast<uint8>(1 << (this->encodedTimeSignature & 7));
		}

		inline uint8 TimeSignatureNumerator() const
		{
			return this->encodedTimeSignature >> 3;
		}

		//Inline
		inline bool IsChordVariationDataAvailable(uint8 cvIndex) const
		{
			return this->chordVariationsWithData & (1 << cvIndex);
		}
	};
}
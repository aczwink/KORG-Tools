/*
 * Copyright (c) 2021-2026 Amir Czwink (amir130@hotmail.de)
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
#pragma once
#include <StdXX.hpp>
#include <libkorg/BankFormat/Style.hpp>
#include <libkorg/BankFormat/PerformanceObject.hpp>
#include <libkorg/Style/StyleElementNumber.hpp>

namespace libKORG
{
	class ITrackView
	{
	public:
		//Destructor
		virtual ~ITrackView() = default;

		//Abstract
		virtual bool IsEmpty() const = 0;
		virtual const Style::MIDI_Track& MIDI_Events() const = 0;
	};

	class IChordVariationView
	{
	public:
		//Destructor
		virtual ~IChordVariationView() = default;

		//Abstract
		virtual bool DoesHaveAnyData() const = 0;
		virtual const ITrackView& GetTrack(AccompanimentTrackNumber trackNumber) const = 0;
	};

	class IStyleElementView
	{
	public:
		//Destructor
		virtual ~IStyleElementView() = default;

		//Abstract
		virtual const IChordVariationView& GetChordVariation(uint8 index) const = 0;
		virtual uint8 GetNumberOfChordVariations() const = 0;
		virtual const Style::StyleTrackData& GetStyleTrackMetaData(AccompanimentTrackNumber trackNumber) const = 0;
	};

	class StyleView
	{
	public:
		//Constructor
		inline StyleView(const Style::StyleData& styleData) : styleData(styleData)
		{
			this->GenerateStyleElementViews(styleData);
		}

		//Properties
		inline StdXX::Math::Rational<uint8> TimeSignature() const
		{
			const auto& data = this->styleData.variation[0].styleElementInfoData;
			return {data.TimeSignatureNumerator(), data.TimeSignatureDenominator()};
		}

		//Inline
		inline const IStyleElementView& GetVariation(uint8 index) const
		{
			return *this->variationViews[index];
		}

		inline const IStyleElementView& GetStyleElement(Style::StyleElementNumber styleElementNumber) const
		{
			return *this->styleElementViews[static_cast<uint32>(styleElementNumber)];
		}

		inline bool IsStyleElementDataAvailable(Style::StyleElementNumber styleElementNumber) const
		{
			return this->styleData.styleInfoData.styleElementsWithData & this->StyleElementNumberToFlag(styleElementNumber);
		}

		inline bool IsStyleElementEnabled(Style::StyleElementNumber styleElementNumber) const
		{
			return this->styleData.styleInfoData.enabledStyleElements & this->StyleElementNumberToFlag(styleElementNumber);
		}

		inline bool IsVariationDataAvailable(uint8 index) const
		{
			return this->styleData.styleInfoData.styleElementsWithData & this->VariationIndexToFlag(index);
		}

		inline bool IsVariationEnabled(uint8 index) const
		{
			return this->styleData.styleInfoData.enabledStyleElements & this->VariationIndexToFlag(index);
		}

	private:
		//Members
		const Style::StyleData& styleData;
		StdXX::StaticArray<StdXX::UniquePointer<IStyleElementView>, 4> variationViews;
		StdXX::StaticArray<StdXX::UniquePointer<IStyleElementView>, 11> styleElementViews;

		//Methods
		void GenerateStyleElementViews(const Style::StyleData& styleData);

		//Inline
		inline uint16 StyleElementNumberToFlag(Style::StyleElementNumber styleElementNumber) const
		{
			uint16 shiftAmount = 4 + (uint16)styleElementNumber;
			return 1 << shiftAmount;
		}

		inline uint8 VariationIndexToFlag(uint8 index) const
		{
			ASSERT(index < 4, u8"Only 4 variations exist");
			return 1 << index;
		}
	};
}
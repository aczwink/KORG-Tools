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
		virtual const Style::MIDI_Track& MIDI_Events() const = 0;
	};

	class IChordVariationView
	{
	public:
		//Destructor
		virtual ~IChordVariationView() = default;

		//Abstract
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
	};

	class StyleView
	{
	public:
		//Constructor
		inline StyleView(const Style::StyleData& styleData) : styleData(styleData)
		{
			this->GenerateStyleElementViews(styleData);
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

		inline bool IsStyleElementEnabled(Style::StyleElementNumber styleElementNumber) const
		{
			uint16 shiftAmount = 4 + (uint16)styleElementNumber;
			return this->styleData._0x1000308_chunk.enabledStyleElements & (1 << shiftAmount);
		}

		inline bool IsVariationEnabled(uint8 index) const
		{
			return this->styleData._0x1000308_chunk.enabledStyleElements & (1 << index);
		}

	private:
		//Members
		const Style::StyleData& styleData;
		StdXX::StaticArray<StdXX::UniquePointer<IStyleElementView>, 4> variationViews;
		StdXX::StaticArray<StdXX::UniquePointer<IStyleElementView>, 11> styleElementViews;

		//Methods
		void GenerateStyleElementViews(const Style::StyleData& styleData);
	};
}
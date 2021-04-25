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
#include <libkorg.hpp>
using namespace libKORG;
using namespace libKORG::Style;
#include "FormattedOutputter.hpp"

class StyleOutputter
{
public:
	//Constructor
	inline StyleOutputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const FullStyle& fullStyle);

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(const ChordTable& chordTable);
	void Output(const StyleObject& style);
	void Output(const StyleData& styleData);
	void Output(const MasterMIDI_Track& midiTrack);
	void Output(const MIDI_Track& midiTrack);
	void Output(uint32 index, const KORG_MIDI_Event& event);
	void Output(const VariationStyleElementData& data);
	void Output(const StyleElementData& data);
	void Output(const ChordVariationData& data);
	void OutputGeneralStyleElementData(const GeneralStyleElementData& data);

	String ToString(StyleElementNumber styleElementNumber) const;
};
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
#include <StdXX.hpp>
//Local
#include <libkorg/BankFormat/Style.hpp>
#include "libkorg/ChunkFormat/ChunkWriter.hpp"
#include "../StyleFormatWriter.hpp"

class StyleFormat0_0V5_0Writer : public libKORG::ChunkWriter, public StyleFormatWriter
{
public:
	//Constructor
	inline StyleFormat0_0V5_0Writer(StdXX::SeekableOutputStream& outputStream) : ChunkWriter(outputStream)
	{
	}

	void Write(const libKORG::Style::StyleData &style) override;

private:
	//Methods
	void WriteMIDITrackMapping(const libKORG::Style::StyleData& styleData);
	void Write0x1000308Chunk(const libKORG::Style::StyleData& styleData);
	void Write0x2000308Chunk(const libKORG::Style::GeneralStyleElementData& styleElementData);
	void WriteChordTable(const libKORG::Style::ChordTable& chordTable);
	void WriteChordVariationData(const libKORG::Style::ChordVariationData& cv);
	void WriteMIDIEvent(const libKORG::Style::KORG_MIDI_Event& event);
	void WriteMIDITrack(const libKORG::Style::MIDI_Track& midiTrack);
	void WriteStyleElement(const libKORG::Style::VariationStyleElementData& styleElementData);
	void WriteStyleElement(const libKORG::Style::StyleElementData& styleElementData);
};
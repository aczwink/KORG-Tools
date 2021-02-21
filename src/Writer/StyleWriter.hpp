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
#include <libkorg/Style.hpp>
#include "ChunkWriter.hpp"

class StyleWriter : public ChunkWriter
{
public:
	//Constructor
	inline StyleWriter(StdXX::SeekableOutputStream& outputStream) : ChunkWriter(outputStream)
	{
	}

	void Write(const libKORG::Style &style);

private:
	//Methods
	void Write0x1000008Chunk(const libKORG::StyleData& styleData);
	void Write0x1000308Chunk(const libKORG::StyleData& styleData);
	void Write0x2000308Chunk(const libKORG::GeneralStyleElementData& styleElementData);
	void WriteChordVariationData(const libKORG::ChordVariationData& cv);
	void WriteMIDIEvent(const libKORG::KORG_MIDI_Event& event);
	void WriteMIDITrack(const libKORG::MIDI_Track& midiTrack);
	void WriteStyleElement(const libKORG::VariationStyleElementData& styleElementData);
	void WriteStyleElement(const libKORG::StyleElementData& styleElementData);
};
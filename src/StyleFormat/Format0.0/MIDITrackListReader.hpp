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
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include <libkorg/Style/StyleData.hpp>

class MIDITrackListReader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline MIDITrackListReader(libKORG::Style::StyleData& data) : data(data)
	{
	}

protected:
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override
	{
		return nullptr;
	}

	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	libKORG::Style::StyleData& data;

	//Methods
	void ReadAccompanymentTrackChunk(StdXX::DataReader &dataReader);
	void ReadBassTrackChunk(StdXX::DataReader &dataReader);
	void ReadDrumOrPercTrackChunk(StdXX::DataReader &dataReader);
	void ReadGuitarTrackChunk(StdXX::DataReader &dataReader);
	void ReadKORG_MIDIEvents(uint16 dataLength, StdXX::DynamicArray<libKORG::Style::KORG_MIDI_Event>& midiEvents, StdXX::DataReader& dataReader);
	void ReadMIDITrackMapping(StdXX::DataReader& dataReader);

	//Inline
	inline libKORG::Style::MIDI_Track& GetNextMIDITrack()
	{
		this->data.midiTracks.Push({});
		return this->data.midiTracks.Last();
	}
};
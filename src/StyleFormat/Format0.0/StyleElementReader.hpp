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
#include <libkorg/Style/StyleData.hpp>
#include <libkorg/ChunkFormat/ChunkReader.hpp>

class StyleElementReader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline StyleElementReader(libKORG::Style::StyleData& data, uint8 currentStyleElementNumber) : data(data), currentStyleElementNumber(currentStyleElementNumber)
	{
		this->nextMasterMIDITrackNumber = -1;
	}

protected:
	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override
	{
		return nullptr;
	}

	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	libKORG::Style::StyleData& data;
	const uint8 currentStyleElementNumber;
	int8 nextMasterMIDITrackNumber;
	uint8 remainingChordVariationFlags;

	//Methods
	void ReadChordTable(libKORG::Style::ChordTable& chordTable, StdXX::DataReader& dataReader);
	void ReadKORG_MIDIEvents(uint16 dataLength, StdXX::DynamicArray<libKORG::Style::KORG_MIDI_Event>& midiEvents, StdXX::DataReader& dataReader);
	void ReadMasterMIDITrack(StdXX::DataReader& dataReader);
	void ReadStyleElementInfoData(const libKORG::ChunkVersion& chunkVersion, libKORG::Style::StyleElementInfoData& styleElementInfoData, StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunk(const libKORG::ChunkVersion& chunkVersion, StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunkV0_0(StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunkV0_1(StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunkV0_2(StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunkV0_3(StdXX::DataReader& dataReader);

	//Inline
	inline auto& GetCurrentChordVariationData()
	{
		if(this->currentStyleElementNumber < 4)
			return this->data.variation[this->currentStyleElementNumber].cv[this->nextMasterMIDITrackNumber];
		return this->data.styleElements[this->currentStyleElementNumber - 4].cv[this->nextMasterMIDITrackNumber];
	}

	inline libKORG::Style::GeneralStyleElementData& GetCurrentStyleElementData()
	{
		uint8 n = this->currentStyleElementNumber;
		if(n < 4)
			return this->data.variation[n];
		return this->data.styleElements[n - 4];
	}

	inline libKORG::Style::MasterMIDI_Track& GetCurrentStyleElementTrack()
	{
		return this->GetCurrentChordVariationData().masterMidiTrack;
	}
};
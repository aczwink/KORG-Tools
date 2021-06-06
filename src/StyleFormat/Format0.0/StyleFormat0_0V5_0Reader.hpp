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
#include <libkorg/BankFormat/Style.hpp>
#include <libkorg/ChunkFormat/ChunkReader.hpp>

class StyleFormat0_0V5_0Reader : public libKORG::ChunkReader
{
public:
	//Constructor
	StyleFormat0_0V5_0Reader();

	//Methods
	libKORG::StyleObject* TakeResult();

protected:
	//Methods
	void ReadDataChunk(const libKORG::ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;
	libKORG::ChunkReader* OnEnteringChunk(const libKORG::ChunkHeader& chunkHeader) override;
	void OnLeavingChunk(const libKORG::ChunkHeader& chunkHeader) override;

private:
	//Members
	uint32 parentChunkId;
	libKORG::Style::StyleData data;
	uint8 nextMIDITrackNumber;
	uint8 currentStyleElementNumber;

	//Methods
	void ReadChordTable(const libKORG::ChunkVersion& chunkVersion, libKORG::Style::ChordTable& chordTable, StdXX::DataReader& dataReader);
	void ReadChordTableV1_1(libKORG::Style::ChordTable& chordTable, StdXX::DataReader& dataReader);
	void ReadMasterMIDITrack(StdXX::DataReader& dataReader);
	void ReadMIDITrackMapping(StdXX::DataReader& dataReader);
	void Read0x1000308Chunk(StdXX::DataReader& dataReader);
	void Read0x2000008Chunk(StdXX::DataReader& dataReader);
	void Read0x3000008Chunk(StdXX::DataReader& dataReader);
	void Read0x4000008Chunk(StdXX::DataReader& dataReader);
	void Read0x5010008Chunk(StdXX::DataReader& dataReader);
	void ReadKORG_MetaEvent(libKORG::Style::KORG_MIDI_Event& event, StdXX::DataReader& dataReader);
	void ReadKORG_MIDIEvents(uint16 dataLength, StdXX::DynamicArray<libKORG::Style::KORG_MIDI_Event>& midiEvents, StdXX::DataReader& dataReader);
	libKORG::Style::StyleTrackData ReadStyleTrackData(StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunk(const libKORG::ChunkVersion& chunkVersion, StdXX::DataReader& dataReader);
	void ReadStyleTrackDataChunkV0_3(StdXX::DataReader& dataReader);

	//Inline
	inline auto& GetCurrentChordVariationData()
	{
		if(this->currentStyleElementNumber < 4)
			return this->data.variation[this->currentStyleElementNumber].cv[this->nextMIDITrackNumber];
		return this->data.styleElements[this->currentStyleElementNumber - 4].cv[this->nextMIDITrackNumber];
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

	inline libKORG::Style::MIDI_Track& GetNextMIDITrack()
	{
		this->data.midiTracks.Push({});
		return this->data.midiTracks.Last();
	}

	inline uint16 ReadLength(StdXX::DataReader& dataReader)
	{
		uint8 b1 = dataReader.ReadByte();
		uint8 b2 = dataReader.ReadByte();

		return (b1 << 7) | (b2 & 0x7F);
	}
};
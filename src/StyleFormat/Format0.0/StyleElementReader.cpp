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
//Class header
#include "StyleElementReader.hpp"
//Local
#include "KORG_MIDI_EventDecoder.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Style;
using namespace StdXX;

//Protected methods
void StyleElementReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.type)
	{
		case 1:
			this->ReadChordTable(chunkHeader.version, this->GetCurrentStyleElementData().chordTable, dataReader);
			break;
		case 2:
			this->ReadStyleTrackDataChunk(chunkHeader.version, dataReader);
			break;
		case 3:
			ASSERT_EQUALS(0, chunkHeader.version.AsUInt16());
			this->ReadMasterMIDITrack(dataReader);
			break;
	}
}

//Private methods
void StyleElementReader::ReadChordTable(const ChunkVersion &chunkVersion, ChordTable &chordTable, DataReader &dataReader)
{
	switch(chunkVersion.AsUInt16())
	{
		case 0x0100:
		case 0x0101:
			this->ReadChordTableV1_1(chordTable, dataReader);
			break;
		case 0x0102:
		case 0x0103:
			this->ReadChordTableV1_1(chordTable, dataReader);
			dataReader.InputStream().FlushTo(*chordTable.unknown.CreateOutputStream());
			break;
	}
}

void StyleElementReader::ReadChordTableV1_1(ChordTable& chordTable, DataReader &dataReader)
{
	chordTable.unknown1 = dataReader.ReadByte();
	chordTable.unknown2 = dataReader.ReadByte();

	ASSERT_EQUALS(24, dataReader.ReadByte()); //number of entries in the chord table

	chordTable.majorCVIndex = dataReader.ReadByte();
	chordTable.sixCVIndex = dataReader.ReadByte();
	chordTable.M7CVIndex = dataReader.ReadByte();
	chordTable.M7b5CVIndex = dataReader.ReadByte();
	chordTable.susCVIndex = dataReader.ReadByte();
	chordTable.sus2CVIndex = dataReader.ReadByte();
	chordTable.M7susCVIndex = dataReader.ReadByte();
	chordTable.mCVIndex = dataReader.ReadByte();
	chordTable.m6CVIndex = dataReader.ReadByte();
	chordTable.m7CVIndex = dataReader.ReadByte();
	chordTable.m7b5CVIndex = dataReader.ReadByte();
	chordTable.mM7CVIndex = dataReader.ReadByte();
	chordTable.sevenCVIndex = dataReader.ReadByte();
	chordTable.seven_b5CVIndex = dataReader.ReadByte();
	chordTable.seven_susCVIndex = dataReader.ReadByte();
	chordTable.dimCVIndex = dataReader.ReadByte();
	chordTable.dimM7CVIndex = dataReader.ReadByte();
	chordTable.sharp5CVIndex = dataReader.ReadByte();
	chordTable.seven_sharp5CVIndex = dataReader.ReadByte();
	chordTable.M7sharp5CVIndex = dataReader.ReadByte();
	chordTable.onePlusFiveCVIndex = dataReader.ReadByte();
	chordTable.onePlusEightCVIndex = dataReader.ReadByte();
	chordTable.b5CVIndex = dataReader.ReadByte();
	chordTable.dim7CVIndex = dataReader.ReadByte();

	chordTable.unknown3 = dataReader.ReadByte();
}

void StyleElementReader::ReadKORG_MIDIEvents(uint16 dataLength, DynamicArray<KORG_MIDI_Event> &midiEvents, DataReader &dataReader)
{
	KORG_MIDI_EventDecoder decoder;
	midiEvents = decoder.DecodeEvents(dataLength, dataReader);
}

void StyleElementReader::ReadMasterMIDITrack(DataReader &dataReader)
{
	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	auto& midiTrack = this->GetCurrentStyleElementTrack();

	midiTrack.unknown1 = dataReader.ReadByte();
	midiTrack.unknown2 = dataReader.ReadByte();
	midiTrack.unknown3 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadByte());

	uint16 dataLength = dataReader.ReadUInt16();
	this->ReadKORG_MIDIEvents(dataLength, midiTrack.events, dataReader);

	auto&cv = this->GetCurrentChordVariationData();

	uint8 nEntries = dataReader.ReadByte();
	cv.trackMapping.Resize(nEntries);
	for (uint8 i = 0; i < nEntries; i++)
	{
		cv.trackMapping[i].type = static_cast<ChordVariationTrackType>(dataReader.ReadByte());
		cv.trackMapping[i].trackNumber = static_cast<AccompanimentTrackNumber>(dataReader.ReadByte());
	}

	this->nextMIDITrackNumber++;
}

void StyleElementReader::ReadStyleTrackDataChunk(const ChunkVersion &chunkVersion, DataReader &dataReader)
{
	switch (chunkVersion.AsUInt16())
	{
		case 0x0002:
			this->ReadStyleTrackDataChunkV0_0(dataReader);
			this->ReadStyleTrackDataChunkV0_1(dataReader);
			this->ReadStyleTrackDataChunkV0_2(dataReader);
			break;
		case 0x0003:
		case 0x0004:
			this->ReadStyleTrackDataChunkV0_0(dataReader);
			this->ReadStyleTrackDataChunkV0_1(dataReader);
			this->ReadStyleTrackDataChunkV0_2(dataReader);
			this->ReadStyleTrackDataChunkV0_3(dataReader);
			break;
	}
}

void StyleElementReader::ReadStyleTrackDataChunkV0_0(DataReader &dataReader)
{
	auto& styleElementData = this->GetCurrentStyleElementData();

	for(StyleTrackData& styleTrackData : styleElementData.styleTrackData)
	{
		styleTrackData.expression = dataReader.ReadByte();

		uint8 msb = dataReader.ReadByte();
		uint8 lsb = dataReader.ReadByte();
		uint8 programChange = dataReader.ReadByte();
		styleTrackData.soundProgramChangeSeq = ProgramChangeSequence(msb, lsb, programChange);

		styleTrackData.keyboardRangeBottom = static_cast<Pitch>(dataReader.ReadByte());
		styleTrackData.keyboardRangeTop = static_cast<Pitch>(dataReader.ReadByte());
	}
}

void StyleElementReader::ReadStyleTrackDataChunkV0_1(DataReader &dataReader)
{
	auto& styleElementData = this->GetCurrentStyleElementData();

	for(uint8 i = 0; i < 8; i++)
	{
		dataReader.ReadBytes(styleElementData.styleTrackData[i].unknown1, sizeof(styleElementData.styleTrackData[i].unknown1));
	}
}

void StyleElementReader::ReadStyleTrackDataChunkV0_2(DataReader &dataReader)
{
	auto& styleElementData = this->GetCurrentStyleElementData();

	for(uint8 i = 0; i < 8; i++)
	{
		styleElementData.styleTrackData[i].unknown2 = dataReader.ReadByte();
	}
}

void StyleElementReader::ReadStyleTrackDataChunkV0_3(DataReader &dataReader)
{
	auto& styleElementData = this->GetCurrentStyleElementData();

	auto output = styleElementData.unknown3.CreateOutputStream();
	dataReader.InputStream().FlushTo(*output);
}
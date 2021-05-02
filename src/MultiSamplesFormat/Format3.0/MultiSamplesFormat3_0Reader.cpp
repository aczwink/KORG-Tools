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
#include "MultiSamplesFormat3_0Reader.hpp"
//Namespaces
using namespace libKORG::MultiSamples;
using namespace StdXX;

void MultiSamplesFormat3_0Reader::Read(DataReader& dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	uint16 versionMaybe = dataReader.ReadUInt16();
	ASSERT((versionMaybe == 2) || (versionMaybe == 3), String::HexNumber(versionMaybe));

	uint16 nSampleEntries = dataReader.ReadUInt16();
	uint16 nDrumSampleEntries = dataReader.ReadUInt16();
	uint16 nOtherOtherEntries = dataReader.ReadUInt16();
	uint16 nMultiSampleEntries = dataReader.ReadUInt16();

	this->ReadSamples(nSampleEntries, dataReader);
	this->ReadDrumSamples(nDrumSampleEntries, dataReader);

	for(uint32 i = 0; i < nOtherOtherEntries; i++)
	{
		uint16 unknown1 = dataReader.ReadUInt16();
		ASSERT_EQUALS(1, dataReader.ReadByte());

		String name = textReader.ReadString(2);

		uint32 unknown3 = dataReader.ReadUInt32();

		uint8 unknown2 = dataReader.ReadByte();
		ASSERT((unknown2 == 0x40) || (unknown2 == 0), "???");

		ASSERT_EQUALS(0, dataReader.ReadUInt32());
		ASSERT_EQUALS(0, dataReader.ReadUInt32());
		ASSERT_EQUALS(0, dataReader.ReadByte());
		uint8 unknown4 = dataReader.ReadByte();
	}

	this->ReadMultiSamples(nMultiSampleEntries, dataReader);
}

//Private methods
void MultiSamplesFormat3_0Reader::ReadDrumSamples(uint16 nDrumSampleEntries, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	this->data.drumSampleEntries.Resize(nDrumSampleEntries);
	for(uint32 i = 0; i < nDrumSampleEntries; i++)
	{
		DrumSampleEntry& drumSampleEntry = this->data.drumSampleEntries[i];

		dataReader.ReadBytes(drumSampleEntry.unknown1, sizeof(drumSampleEntry.unknown1));
		drumSampleEntry.name = textReader.ReadZeroTerminatedString(16);
		dataReader.ReadBytes(drumSampleEntry.unknown2, sizeof(drumSampleEntry.unknown2));
	}
}

void MultiSamplesFormat3_0Reader::ReadMultiSamples(uint16 nMultiSampleEntries, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::Latin1);

	this->data.multiSampleEntries.Resize(nMultiSampleEntries);
	for(uint32 i = 0; i < nMultiSampleEntries; i++)
	{
		MultiSampleEntry& multiSampleEntry = this->data.multiSampleEntries[i];

		ASSERT_EQUALS(0x80, dataReader.ReadByte());

		multiSampleEntry.unknown1 = dataReader.ReadByte();
		multiSampleEntry.name = textReader.ReadZeroTerminatedString(24);
		dataReader.ReadBytes(multiSampleEntry.unknown2, sizeof(multiSampleEntry.unknown2));
	}
}

void MultiSamplesFormat3_0Reader::ReadSamples(uint16 nSampleEntries, DataReader& dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	this->data.sampleEntries.Resize(nSampleEntries);
	for(uint32 i = 0; i < this->data.sampleEntries.GetNumberOfElements(); i++)
	{
		SampleEntry& sampleEntry = this->data.sampleEntries[i];

		dataReader.ReadBytes(sampleEntry.unknown1, sizeof(sampleEntry.unknown1));
		sampleEntry.unknown6 = dataReader.ReadByte();
		dataReader.ReadBytes(sampleEntry.unknown2, sizeof(sampleEntry.unknown2));
		sampleEntry.name = textReader.ReadZeroTerminatedString(16);
		dataReader.ReadBytes(sampleEntry.unknown3, sizeof(sampleEntry.unknown3));
		dataReader.ReadBytes(sampleEntry.unknown4, sizeof(sampleEntry.unknown4));

		sampleEntry.originalNote = dataReader.ReadByte();
	}
}
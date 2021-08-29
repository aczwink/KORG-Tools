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
//Class header
#include "MultiSamplesFormat1_0Reader.hpp"
//Namespaces
using namespace libKORG::MultiSamples;
using namespace StdXX;

//Public methods
void MultiSamplesFormat1_0Reader::Read(DataReader &dataReader)
{
	uint16 versionMaybe = dataReader.ReadUInt16();
	ASSERT((versionMaybe == 2) || (versionMaybe == 3), String::HexNumber(versionMaybe));

	uint16 nSampleEntries = dataReader.ReadUInt16();
	uint16 nDrumSampleEntries = dataReader.ReadUInt16();
	uint16 nKeyboardZones = dataReader.ReadUInt16();
	uint16 nMultiSampleEntries = dataReader.ReadUInt16();

	this->ReadSamples(nSampleEntries, dataReader);
	this->ReadDrumSamples(nDrumSampleEntries, dataReader);
	this->ReadKeyboardZones(nKeyboardZones, dataReader);
	this->ReadMultiSamples(nMultiSampleEntries, dataReader);
}

//Protected methods
void MultiSamplesFormat1_0Reader::ReadDrumSampleUnknowns3And4(DrumSampleEntry &drumSampleEntry, DataReader &dataReader)
{
}

void MultiSamplesFormat1_0Reader::ReadDrumSampleUnknowns9To12(DrumSampleEntry &drumSampleEntry, DataReader &dataReader)
{
}

int16 MultiSamplesFormat1_0Reader::ReadSampleUnknown2(DataReader &dataReader)
{
	return dataReader.ReadInt8();
}

void MultiSamplesFormat1_0Reader::ReadSampleUnknown14(SampleEntry& sampleEntry, DataReader &dataReader)
{
}

//Private methods
void MultiSamplesFormat1_0Reader::ReadDrumSamples(uint16 nDrumSampleEntries, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::Latin1);

	this->data.drumSampleEntries.Resize(nDrumSampleEntries);
	for(uint32 i = 0; i < nDrumSampleEntries; i++)
	{
		DrumSampleEntry& drumSampleEntry = this->data.drumSampleEntries[i];

		drumSampleEntry.sampleIndexLeft = dataReader.ReadInt16();
		drumSampleEntry.sampleIndexRight = dataReader.ReadInt16();

		this->ReadDrumSampleUnknowns3And4(drumSampleEntry, dataReader);

		drumSampleEntry.bankNumber = dataReader.ReadByte();
		drumSampleEntry.family = static_cast<DrumSampleFamily>(dataReader.ReadInt8());
		drumSampleEntry.unknown7 = dataReader.ReadByte();
		drumSampleEntry.unknown8 = dataReader.ReadInt8();

		drumSampleEntry.name = textReader.ReadZeroTerminatedString(24);

		this->ReadDrumSampleUnknowns9To12(drumSampleEntry, dataReader);

		drumSampleEntry.id = dataReader.ReadUInt64();
	}
}

void MultiSamplesFormat1_0Reader::ReadExtendedSampleInfo(SampleEntry &sampleEntry, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::Latin1);

	sampleEntry.name = textReader.ReadZeroTerminatedString(16);
	sampleEntry.id = dataReader.ReadUInt64();

	sampleEntry.unknown15 = dataReader.ReadByte();
	sampleEntry.unknown16 = dataReader.ReadByte();
	sampleEntry.unknown17 = dataReader.ReadUInt32();

	sampleEntry.originalNote = dataReader.ReadByte();
}

void MultiSamplesFormat1_0Reader::ReadKeyboardZones(uint16 nEntries, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::ASCII);

	this->data.keyboardZones.Resize(nEntries);
	for(uint32 i = 0; i < nEntries; i++)
	{
		KeyboardZone& keyboardZone = this->data.keyboardZones[i];

		keyboardZone.sampleNumber = dataReader.ReadInt16();

		keyboardZone.unknown1 = dataReader.ReadByte();

		keyboardZone.to = dataReader.ReadByte();
		keyboardZone.originalNote = dataReader.ReadByte();

		keyboardZone.unknown3 = dataReader.ReadByte();

		keyboardZone.pitch = dataReader.ReadInt16();
		keyboardZone.level = dataReader.ReadByte();
		keyboardZone.unknown4 = dataReader.ReadByte();

		ASSERT_EQUALS(0, dataReader.ReadUInt32());
		ASSERT_EQUALS(0, dataReader.ReadUInt32());
		ASSERT_EQUALS(0, dataReader.ReadByte());

		keyboardZone.unknown5 = dataReader.ReadByte();
	}
}

void MultiSamplesFormat1_0Reader::ReadMultiSamples(uint16 nMultiSampleEntries, DataReader &dataReader)
{
	TextReader textReader(dataReader.InputStream(), TextCodecType::Latin1);

	this->data.multiSampleEntries.Resize(nMultiSampleEntries);
	for(uint32 i = 0; i < nMultiSampleEntries; i++)
	{
		MultiSampleEntry& multiSampleEntry = this->data.multiSampleEntries[i];

		multiSampleEntry.unknown1 = dataReader.ReadUInt16();

		multiSampleEntry.name = textReader.ReadZeroTerminatedString(24);
		multiSampleEntry.keyZoneBaseIndex = dataReader.ReadUInt16();
		dataReader.ReadBytes(multiSampleEntry.keyZoneIndex, sizeof(multiSampleEntry.keyZoneIndex));
		multiSampleEntry.nKeyZones = dataReader.ReadByte();

		multiSampleEntry.unknown3 = dataReader.ReadByte();

		multiSampleEntry.id = dataReader.ReadUInt64();
	}
}

void MultiSamplesFormat1_0Reader::ReadSamples(uint16 nSampleEntries, DataReader &dataReader)
{
	this->data.sampleEntries.Resize(nSampleEntries);
	for(uint32 i = 0; i < this->data.sampleEntries.GetNumberOfElements(); i++)
	{
		SampleEntry& sampleEntry = this->data.sampleEntries[i];

		sampleEntry.unknown1 = dataReader.ReadInt16();
		sampleEntry.unknown2 = this->ReadSampleUnknown2(dataReader);

		sampleEntry.packedData = dataReader.ReadUInt16();

		sampleEntry.bankNumber = dataReader.ReadByte();
		sampleEntry.unknown8 = dataReader.ReadUInt32();

		for(uint8 j = 0; j < 8; j++)
		{
			sampleEntry.unknown9[j] = dataReader.ReadUInt32();
		}

		sampleEntry.unknown5 = dataReader.ReadByte();
		sampleEntry.unknown10 = dataReader.ReadUInt32();
		sampleEntry.unknown11 = dataReader.ReadUInt32();
		sampleEntry.unknown12 = dataReader.ReadUInt32();
		sampleEntry.unknown13 = dataReader.ReadUInt32();

		sampleEntry.compressionCoefficients[0] = dataReader.ReadInt16();
		sampleEntry.compressionCoefficients[1] = dataReader.ReadInt16();

		this->ReadSampleUnknown14(sampleEntry, dataReader);
		this->ReadExtendedSampleInfo(sampleEntry, dataReader);
	}
}
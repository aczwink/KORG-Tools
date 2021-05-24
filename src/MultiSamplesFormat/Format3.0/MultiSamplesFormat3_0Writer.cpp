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
#include "MultiSamplesFormat3_0Writer.hpp"
//Namespaces
using namespace StdXX;

//Public methods
void MultiSamplesFormat3_0Writer::Write(const libKORG::MultiSamples::MultiSamplesData& data)
{
	this->dataWriter.WriteUInt16(3);

	this->dataWriter.WriteUInt16(data.sampleEntries.GetNumberOfElements());
	this->dataWriter.WriteUInt16(data.drumSampleEntries.GetNumberOfElements());
	this->dataWriter.WriteUInt16(data.keyboardZones.GetNumberOfElements());
	this->dataWriter.WriteUInt16(data.multiSampleEntries.GetNumberOfElements());

	for(const auto& entry : data.sampleEntries)
		this->Write(entry);
	for(const auto& entry : data.drumSampleEntries)
		this->Write(entry);
	for(const auto& entry : data.keyboardZones)
		this->Write(entry);
	for(const auto& entry : data.multiSampleEntries)
		this->Write(entry);
}

//Private methods
void MultiSamplesFormat3_0Writer::Write(const libKORG::MultiSamples::DrumSampleEntry& drumSampleEntry)
{
	this->dataWriter.WriteInt16(drumSampleEntry.unknown1);
	this->dataWriter.WriteInt16(drumSampleEntry.unknown2);
	this->dataWriter.WriteInt16(drumSampleEntry.unknown3);
	this->dataWriter.WriteInt16(drumSampleEntry.unknown4);
	this->dataWriter.WriteByte(drumSampleEntry.unknown5);
	this->dataWriter.WriteByte(drumSampleEntry.unknown6);
	this->dataWriter.WriteByte(drumSampleEntry.unknown7);
	this->dataWriter.WriteByte(drumSampleEntry.unknown8);

	this->textWriter.WriteFixedLengthString(drumSampleEntry.name, 24);

	this->dataWriter.WriteByte(drumSampleEntry.unknown9);
	this->dataWriter.WriteByte(drumSampleEntry.unknown10);
	this->dataWriter.WriteByte(drumSampleEntry.unknown11);
	this->dataWriter.WriteByte(drumSampleEntry.unknown12);

	this->dataWriter.WriteUInt64(drumSampleEntry.id);
}

void MultiSamplesFormat3_0Writer::Write(const libKORG::MultiSamples::KeyboardZone& keyboardZone)
{
	this->dataWriter.WriteInt16(keyboardZone.sampleNumber);

	this->dataWriter.WriteByte(1);

	this->dataWriter.WriteByte(keyboardZone.to.Encode());
	this->dataWriter.WriteByte(keyboardZone.originalNote.Encode());

	this->dataWriter.WriteByte(keyboardZone.unknown3);

	this->dataWriter.WriteInt16(keyboardZone.pitch);
	this->dataWriter.WriteByte(keyboardZone.level);

	this->dataWriter.WriteByte(keyboardZone.unknown4);

	this->dataWriter.WriteUInt32(0);
	this->dataWriter.WriteUInt32(0);
	this->dataWriter.WriteByte(0);
	this->dataWriter.WriteByte(keyboardZone.unknown5);
}

void MultiSamplesFormat3_0Writer::Write(const libKORG::MultiSamples::MultiSampleEntry& multiSampleEntry)
{
	this->dataWriter.WriteUInt16(multiSampleEntry.unknown1);

	this->textWriter.WriteFixedLengthString(multiSampleEntry.name, 24);

	this->dataWriter.WriteUInt16(multiSampleEntry.keyZoneBaseIndex);
	this->dataWriter.WriteBytes(multiSampleEntry.keyZoneIndex, sizeof(multiSampleEntry.keyZoneIndex));
	this->dataWriter.WriteByte(multiSampleEntry.nKeyZones);

	this->dataWriter.WriteByte(multiSampleEntry.unknown3);

	this->dataWriter.WriteUInt64(multiSampleEntry.id);
}

void MultiSamplesFormat3_0Writer::Write(const libKORG::MultiSamples::SampleEntry &sampleEntry)
{
	this->dataWriter.WriteInt16(sampleEntry.unknown1);
	this->dataWriter.WriteInt16(sampleEntry.unknown2);

	this->dataWriter.WriteUInt16(sampleEntry.packedData.Encoded().value);

	this->dataWriter.WriteByte(sampleEntry.unknown4);
	this->dataWriter.WriteUInt32(sampleEntry.unknown8);

	for(uint8 j = 0; j < 8; j++)
	{
		this->dataWriter.WriteUInt32(sampleEntry.unknown9[j]);
	}

	this->dataWriter.WriteByte(sampleEntry.unknown5);
	this->dataWriter.WriteUInt32(sampleEntry.unknown10);
	this->dataWriter.WriteUInt32(sampleEntry.unknown11);
	this->dataWriter.WriteUInt32(sampleEntry.unknown12);
	this->dataWriter.WriteUInt32(sampleEntry.unknown13);

	this->dataWriter.WriteInt16(sampleEntry.compressionCoefficients[0]);
	this->dataWriter.WriteInt16(sampleEntry.compressionCoefficients[1]);

	this->dataWriter.WriteUInt32(sampleEntry.unknown14);

	this->textWriter.WriteFixedLengthString(sampleEntry.name, 16);
	this->dataWriter.WriteUInt64(sampleEntry.id);

	this->dataWriter.WriteByte(sampleEntry.unknown15);
	this->dataWriter.WriteByte(sampleEntry.unknown16);
	this->dataWriter.WriteUInt32(sampleEntry.unknown17);

	this->dataWriter.WriteByte(sampleEntry.originalNote.Encode());
}

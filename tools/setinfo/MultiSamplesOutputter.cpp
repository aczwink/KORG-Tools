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
#include "MultiSamplesOutputter.hpp"

//Public methods
void MultiSamplesOutputter::Output(const MultiSamplesObject& multiSamplesObject) const
{
	{
		Section sampleEntriesSection(u8"Sample entries", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.sampleEntries.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.sampleEntries[i]);
	}

	{
		Section sampleEntriesSection(u8"Drum sample entries", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.drumSampleEntries.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.drumSampleEntries[i]);
	}

	{
		Section sampleEntriesSection(u8"Keyboard zones", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.keyboardZones.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.keyboardZones[i]);
	}

	{
		Section sampleEntriesSection(u8"Multisample entries", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.multiSampleEntries.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.multiSampleEntries[i]);
	}
}

//Private methods
void MultiSamplesOutputter::Output(uint32 index, const DrumSampleEntry& drumSampleEntry) const
{
	Section section(u8"Drum sample entry " + String::Number(index), this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", drumSampleEntry.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", drumSampleEntry.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", drumSampleEntry.unknown3);
	this->formattedOutputter.OutputProperty(u8"unknown4", drumSampleEntry.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", drumSampleEntry.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown6", drumSampleEntry.unknown6);
	this->formattedOutputter.OutputProperty(u8"unknown7", drumSampleEntry.unknown7);
	this->formattedOutputter.OutputProperty(u8"unknown8", drumSampleEntry.unknown8);

	this->formattedOutputter.OutputProperty(u8"name", drumSampleEntry.name);

	this->formattedOutputter.OutputProperty(u8"unknown9", drumSampleEntry.unknown9);
	this->formattedOutputter.OutputProperty(u8"unknown10", drumSampleEntry.unknown10);
	this->formattedOutputter.OutputProperty(u8"unknown11", drumSampleEntry.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown12", drumSampleEntry.unknown12);

	this->formattedOutputter.OutputProperty(u8"ID", String::HexNumber(drumSampleEntry.id, 16));
}

void MultiSamplesOutputter::Output(uint32 index, const MultiSampleEntry& multiSampleEntry) const
{
	Section section(u8"Multisample entry " + String::Number(index), this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", multiSampleEntry.unknown1);
	this->formattedOutputter.OutputProperty(u8"name", multiSampleEntry.name);
	this->formattedOutputter.OutputProperty(u8"keyZoneBaseIndex", multiSampleEntry.keyZoneBaseIndex);

	for(uint8 i = 0; i < 128; i++)
	{
		Section subSection(u8"Key entry " + String::Number(i), this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"Pitch", Pitch(i));
		this->formattedOutputter.OutputProperty(u8"keyZoneIndex", multiSampleEntry.keyZoneIndex[i]);
	}

	this->formattedOutputter.OutputProperty(u8"nKeyZones", multiSampleEntry.nKeyZones);
	this->formattedOutputter.OutputProperty(u8"unknown3", multiSampleEntry.unknown3);
	this->formattedOutputter.OutputProperty(u8"id", String::HexNumber(multiSampleEntry.id, 16));
}

void MultiSamplesOutputter::Output(uint32 index, const SampleEntry& sampleEntry) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: refix me
	/*
	Section section(u8"Sample entry " + String::Number(index), this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", sampleEntry.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", sampleEntry.unknown2);

	this->formattedOutputter.OutputProperty(u8"sampleType", (uint8)sampleEntry.sampleType);
	this->formattedOutputter.OutputProperty(u8"interpolationMode", (uint8)sampleEntry.interpolationMode);
	this->formattedOutputter.OutputProperty(u8"flags", sampleEntry.flags.encodedFlags);

	this->formattedOutputter.OutputProperty(u8"unknown4", sampleEntry.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown8", sampleEntry.unknown8);
	for(uint32 i = 0; i < 8; i++)
		this->formattedOutputter.OutputProperty(u8"unknown9_ " + String::Number(i), sampleEntry.unknown9[i]);
	this->formattedOutputter.OutputProperty(u8"unknown5", sampleEntry.unknown5);
	this->formattedOutputter.OutputProperty(u8"unknown10", sampleEntry.unknown10);
	this->formattedOutputter.OutputProperty(u8"unknown11", sampleEntry.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown12", sampleEntry.unknown12);
	this->formattedOutputter.OutputProperty(u8"unknown13", sampleEntry.unknown13);

	this->formattedOutputter.OutputProperty(u8"compressionCoefficient1", sampleEntry.compressionCoefficients[0]);
	this->formattedOutputter.OutputProperty(u8"compressionCoefficient2", sampleEntry.compressionCoefficients[1]);

	this->formattedOutputter.OutputProperty(u8"unknown14", sampleEntry.unknown14);

	this->formattedOutputter.OutputProperty(u8"name", sampleEntry.name);
	this->formattedOutputter.OutputProperty(u8"ID", String::HexNumber(sampleEntry.id, 16));

	this->formattedOutputter.OutputProperty(u8"unknown15", sampleEntry.unknown15);
	this->formattedOutputter.OutputProperty(u8"unknown16", sampleEntry.unknown16);
	this->formattedOutputter.OutputProperty(u8"unknown17", sampleEntry.unknown17);

	this->formattedOutputter.OutputProperty(u8"originalNote", sampleEntry.originalNote);*/
}

void MultiSamplesOutputter::Output(uint32 index, const KeyboardZone& keyboardZone) const
{
	Section section(u8"Keyboard zone " + String::Number(index), this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"sampleNumber", keyboardZone.sampleNumber);
	this->formattedOutputter.OutputProperty(u8"to", keyboardZone.to);
	this->formattedOutputter.OutputProperty(u8"originalNote", keyboardZone.originalNote);
	this->formattedOutputter.OutputProperty(u8"unknown3", keyboardZone.unknown3);
	this->formattedOutputter.OutputProperty(u8"pitch", keyboardZone.pitch);
	this->formattedOutputter.OutputProperty(u8"level", keyboardZone.level);
	this->formattedOutputter.OutputProperty(u8"unknown4", keyboardZone.unknown4);
	this->formattedOutputter.OutputProperty(u8"unknown5", keyboardZone.unknown5);
}

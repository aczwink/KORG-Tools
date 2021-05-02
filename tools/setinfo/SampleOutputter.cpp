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
#include "SampleOutputter.hpp"

//Public methods
void SampleOutputter::Output(const AbstractSample &sampleObject) const
{
	if(IS_INSTANCE_OF(&sampleObject, EncryptedSample))
	{
		Section section(u8"Encrypted sample", this->formattedOutputter);
		return;
	}

	const SampleObject& sample = dynamic_cast<const SampleObject&>(sampleObject);
	this->Output(sample.data);
}

//Private methods
void SampleOutputter::Output(const SampleData& sampleData) const
{
	Section section(u8"Sample", this->formattedOutputter);

	this->formattedOutputter.OutputUnknownProperties(sampleData.unknown1, sizeof(sampleData.unknown1));

	this->formattedOutputter.OutputProperty(u8"ID", String::HexNumber(sampleData.id[0] << 8 | sampleData.id[1], 2));

	this->formattedOutputter.OutputUnknownProperties(sampleData.unknown11, sizeof(sampleData.unknown11));
	this->formattedOutputter.OutputProperty(u8"Sample format", (uint8)sampleData.sampleFormat);
	this->formattedOutputter.OutputProperty(u8"Sample rate", sampleData.sampleRate);
	this->formattedOutputter.OutputUnknownProperties(sampleData.unknown2, sizeof(sampleData.unknown2));
	this->formattedOutputter.OutputProperty(u8"Sample count", sampleData.samples.GetNumberOfElements());
}

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
#include "PCMFormat0_0Reader.hpp"
//Namespaces
using namespace libKORG::Sample;
using namespace StdXX;

void PCMFormat0_0Reader::Read(DataReader& dataReader)
{
	dataReader.ReadBytes(this->data.unknown1, sizeof(this->data.unknown1));
	dataReader.ReadBytes(this->data.id, sizeof(this->data.id));
	dataReader.ReadBytes(this->data.unknown11, sizeof(this->data.unknown11));

	ASSERT_EQUALS(1, dataReader.ReadByte()); //maybe number of channels?
	this->data.sampleFormat = static_cast<SampleFormat>(dataReader.ReadByte());
	this->data.sampleRate = dataReader.ReadUInt32();
	uint32 nSamples = dataReader.ReadUInt32();

	dataReader.ReadBytes(this->data.unknown2, sizeof(this->data.unknown2));

	uint32 samplesSize;
	switch (this->data.sampleFormat)
	{
		case SampleFormat::S16BE:
			samplesSize = 2;
			break;
		case SampleFormat::S8:
			samplesSize = 1;
			break;
	}
	samplesSize *= nSamples;

	dataReader.InputStream().FlushTo(*this->data.sampleBuffer.CreateOutputStream(), samplesSize);
}

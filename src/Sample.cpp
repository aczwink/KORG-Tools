/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg/Sample.hpp>
//Namespaces
using namespace StdXX;
using namespace libKORG;

//Public methods
void Sample::WriteUnknownData(OutputStream& outputStream) const
{
	DataWriter dw(true, outputStream);
	dw.WriteUInt64(this->unknown);
	outputStream.WriteBytes(this->unknown2, sizeof(this->unknown2));
}

//Private methods
void Sample::ReadData(InputStream &inputStream)
{
	DataReader dataReader(true, inputStream);

	this->unknown = dataReader.ReadUInt64();
	ASSERT_EQUALS(1, dataReader.ReadByte()); //maybe number of channels?
	ASSERT_EQUALS(16, dataReader.ReadByte()); //maybe bits per sample?
	this->sampleRate = dataReader.ReadUInt32();
	uint32 nSamples = dataReader.ReadUInt32();

	inputStream.ReadBytes(this->unknown2, sizeof(this->unknown2));

	this->samples.Resize(nSamples);
	for(uint32 i = 0; i < nSamples; i++)
	{
		this->samples[i] = dataReader.ReadUInt16();
	}

	ASSERT_EQUALS(true, inputStream.IsAtEnd());
}
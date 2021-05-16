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
#include "PCMFormat0_0Writer.hpp"
//Namespaces
using namespace libKORG::Sample;

//Public methods
void PCMFormat0_0Writer::Write(const SampleData &sampleData)
{
	this->dataWriter.WriteUInt64(sampleData.id);
	this->dataWriter.WriteByte(1);
	this->dataWriter.WriteByte(static_cast<byte>(sampleData.sampleFormat));
	this->dataWriter.WriteUInt32(sampleData.sampleRate);
	this->dataWriter.WriteUInt32(sampleData.nSamples);

	this->dataWriter.WriteUInt32(sampleData.unknown1);
	for(uint8 i = 0; i < 7; i++)
	{
		this->dataWriter.WriteUInt32(sampleData.unknown2[i]);
	}
	this->dataWriter.WriteUInt32(sampleData.loopStart);
	this->dataWriter.WriteUInt32(sampleData.unknown3);
	this->dataWriter.WriteUInt32(sampleData.unknown4);
	this->dataWriter.WriteUInt32(sampleData.unknown5);

	sampleData.sampleBuffer.CreateInputStream()->FlushTo(this->dataWriter.Stream());
}
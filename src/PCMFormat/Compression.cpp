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
#include <libkorg/Sample/Compression.hpp>
//Namespaces
using namespace libKORG;

void Sample::Decompress(const uint8* compressedSamples, int16* uncompressedSamples, uint32 nSamples)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me

	int16 lastSample = 0;
	for(uint32 i = 0; i < nSamples; i++)
	{
		int8 v8 = *compressedSamples;

		lastSample += v8;
		*uncompressedSamples++ = lastSample;// * 32767.0;
		//StdXX::stdOut << (i+1) << ". " << *compressedSamples << " " << (int8)*compressedSamples << " -> " << lastSample << StdXX::endl;
		compressedSamples++;
	}
}
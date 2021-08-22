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
//Local
#include <libkorg/BankFormat/MultiSamplesObject.hpp>
#include "Format3.0/MultiSamplesFormat3_0Reader.hpp"
#include "Format1.0/MultiSamplesFormat1_0Reader.hpp"
#include "Format5.0/MultiSamplesFormat5_0Reader.hpp"

class MultiSamplesReader
{
public:
	BankFormat::BankObject *Read(const ChunkVersion& chunkVersion, DataReader& dataReader)
	{
		switch (chunkVersion.AsUInt16())
		{
			case 0x0100:
			{
				MultiSamplesFormat1_0Reader multiSamplesFormat10Reader;
				multiSamplesFormat10Reader.Read(dataReader);
				return new MultiSamplesObject(Move(multiSamplesFormat10Reader.data));
			}
			case 0x0300:
			{
				MultiSamplesFormat3_0Reader multiSamplesFormat30Reader;
				multiSamplesFormat30Reader.Read(dataReader);
				return new MultiSamplesObject(Move(multiSamplesFormat30Reader.data));
			}
			case 0x0500:
			{
				MultiSamplesFormat5_0Reader multiSamplesFormat50Reader;
				multiSamplesFormat50Reader.Read(dataReader);
				return new MultiSamplesObject(Move(multiSamplesFormat50Reader.data));
			}
			default:
				stdErr << u8"Unknown multi samples version: " << String::HexNumber(chunkVersion.AsUInt16(), 4) << endl;
		}

		return nullptr;


	}
};
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
#include <libkorg/BankFormat/BankObject.hpp>
#include <libkorg/ChunkFormat/ChunkFormat.hpp>
#include "Format0.0/PCMFormat0_0Reader.hpp"

class PCMReader
{
public:
	AbstractSample *Read(const libKORG::ChunkVersion& chunkVersion, StdXX::DataReader& dataReader)
	{
		switch(chunkVersion.AsUInt16())
		{
			case 0x0000:
			{
				PCMFormat0_0Reader pcmFormat00Reader;
				pcmFormat00Reader.Read(dataReader);
				return new SampleObject(StdXX::Move(pcmFormat00Reader.data));
			}
			break;
			default:
			{
				StdXX::stdErr << u8"Unknown PCM version: " << chunkVersion.major << u8"." << chunkVersion.minor << u8" skipping..." << StdXX::endl;
				StdXX::NullOutputStream nullOutputStream;
				dataReader.InputStream().FlushTo(nullOutputStream);
			}
		}

		return nullptr;
	}
};
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
#include "PerformanceFormatV0_3Reader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Performance::V0;
using namespace StdXX;

//Protected methods
libKORG::ChunkReader *PerformanceFormatV0_3Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void PerformanceFormatV0_3Reader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x00010008:
			this->Read0x00000008Chunk(dataReader);
			break;
		case 0x02000108:
			this->Read0x02Chunk(chunkHeader.version.minor, dataReader);
			break;
	}
}

void PerformanceFormatV0_3Reader::ReadFX(FX &fx, DataReader &dataReader)
{
	dataReader.ReadBytes(fx.unknown9, sizeof(fx.unknown9));
}
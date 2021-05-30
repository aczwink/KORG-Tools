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
#include "UnknownAdditionalReader.hpp"
//Namespaces
using namespace libKORG;

libKORG::ChunkReader *UnknownAdditionalReader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	return nullptr;
}

void UnknownAdditionalReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x1B000008:
		{
			auto& chunk = this->generalData._0x1B000008_data;

			chunk.unknown1 = dataReader.ReadByte();
			chunk.unknown2 = dataReader.ReadByte();
			chunk.unknown3 = dataReader.ReadByte();
		}
		break;
		case 0x1A000008:
		{
			auto& chunk = this->generalData._0x1A000008_data;

			chunk.unknown1 = dataReader.ReadByte();
			chunk.unknown2 = dataReader.ReadByte();
		}
		break;
	}
}

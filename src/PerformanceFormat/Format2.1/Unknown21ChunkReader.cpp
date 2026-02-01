/*
 * Copyright (c) 2026 Amir Czwink (amir130@hotmail.de)
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
#include "Unknown21ChunkReader.hpp"
//Namespaces
using namespace StdXX;

libKORG::ChunkReader *Unknown21ChunkReader::OnEnteringChunk(const libKORG::ChunkHeader& chunkHeader)
{
	switch(chunkHeader.id)
	{
		case 0x23000000:
			return &this->unknown23ChunkReader;
	}
	return nullptr;
}

void Unknown21ChunkReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, DataReader &dataReader)
{
}

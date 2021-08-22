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
#include "UnknownAdditionalAccompanimentSettingsReader.hpp"
//Namespaces
using namespace StdXX;

libKORG::ChunkReader* UnknownAdditionalAccompanimentSettingsReader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

void UnknownAdditionalAccompanimentSettingsReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x00000008:
		{
			auto dest = this->unknownAccSettings._0x00000008_chunk.CreateOutputStream();
			dataReader.InputStream().FlushTo(*dest);
		}
		break;
		default:
			ASSERT(false, String::HexNumber(chunkHeader.id));
	}
}
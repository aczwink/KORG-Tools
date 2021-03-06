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
#include "StyleFormat0_0V4_0Reader.hpp"
//Local
#include "StyleInfoDataReader.hpp"
#include "MIDITrackListReader.hpp"
#include "StyleElementReader.hpp"
//Namespaces
using namespace libKORG;

//Protected methods
libKORG::ChunkReader *StyleFormat0_0V4_0Reader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	switch(chunkHeader.id)
	{
		case 0x02000000:
			this->subReader = new MIDITrackListReader(this->data);
			return this->subReader.operator->();
		case 0x03000000:
			this->subReader = new StyleElementReader(this->data, this->currentStyleElementNumber);
			return this->subReader.operator->();
	}

	return nullptr;
}

void StyleFormat0_0V4_0Reader::OnLeavingChunk(const ChunkHeader &chunkHeader)
{
	if(chunkHeader.id == 0x3000000)
	{
		this->currentStyleElementNumber++;
	}
}

void StyleFormat0_0V4_0Reader::ReadDataChunk(const ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{
	switch(chunkHeader.id)
	{
		case 0x01000208:
		{
			StyleInfoDataReader styleInfoDataReader;
			styleInfoDataReader.Read(this->data.styleInfoData, chunkHeader.version.minor, dataReader);
		}
		break;
	}
}

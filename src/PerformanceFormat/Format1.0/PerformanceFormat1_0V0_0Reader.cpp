/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "PerformanceFormat1_0V0_0Reader.hpp"
//Local
#include <libkorg/ProgramChangeSequence.hpp>
#include "../../Reading/PerformanceDataFormat.hpp"
#include "AccompanimentSettingsReader.hpp"
#include "KeyboardSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
libKORG::Performance *PerformanceFormat1_0V0_0Reader::TakePerformanceResult()
{
	ASSERT_EQUALS(1, this->perfIndex);

	return new Performance(Move(this->unknownChunksAtBeginning), Move(this->accompanimentSettings), Move(this->keyboardSettings[0]));
}

libKORG::SingleTouchSettings *PerformanceFormat1_0V0_0Reader::TakeSTSResult()
{
	ASSERT_EQUALS(4, this->perfIndex);

	return new SingleTouchSettings(this->max9version, Move(this->unknownChunksAtBeginning), Move(this->accompanimentSettings), Move(this->keyboardSettings), Move(this->unknownChunksAtEnd));
}

//Protected methods
ChunkReader* PerformanceFormat1_0V0_0Reader::OnEnteringChunk(const ChunkHeader &chunkHeader)
{
	switch(chunkHeader.type)
	{
		case 2:
			return &this->accompanimentSettingsReader;
		case 10:
			this->keyboardSettingsReader = new KeyboardSettingsReader(keyboardSettings, perfIndex);
			return this->keyboardSettingsReader.operator->();
	}
	return this;
}

void PerformanceFormat1_0V0_0Reader::OnLeavingChunk(const ChunkHeader &chunkHeader)
{
	if(chunkHeader.type == 10)
	{
		this->perfIndex++;
	}

	ChunkReader::OnLeavingChunk(chunkHeader);
}

void PerformanceFormat1_0V0_0Reader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.type)
	{
		case 4:
		case 32:
			this->unknownChunksAtBeginning.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
			break;
		case 26:
		case 27:
		case 33:
			this->unknownChunksAtEnd.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
		break;
	}
}

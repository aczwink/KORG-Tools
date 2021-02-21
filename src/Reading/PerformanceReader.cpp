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
#include "PerformanceReader.hpp"
//Local
#include <libkorg/ProgramChangeSequence.hpp>
#include "PerformanceDataFormat.hpp"
#include "AccompanimentSettingsReader.hpp"
#include "KeyboardSettingsReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
libKORG::Performance *PerformanceReader::TakePerformanceResult()
{
	ASSERT_EQUALS(1, this->perfIndex);

	return new Performance(Move(this->unknownChunksAtBeginning), Move(this->accompanimentSettings), Move(this->keyboardSettings[0]));
}

libKORG::SingleTouchSettings *PerformanceReader::TakeSTSResult()
{
	ASSERT_EQUALS(4, this->perfIndex);

	return new SingleTouchSettings(this->version, Move(this->unknownChunksAtBeginning), Move(this->accompanimentSettings), Move(this->keyboardSettings), Move(this->unknownChunksAtEnd));
}

//Protected methods
String PerformanceReader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/PERFORMANCE/";
}

bool PerformanceReader::IsDataChunk(const ChunkHeader &chunkHeader)
{
	switch(chunkHeader.type)
	{
		case 2:
		case 10:
			return true;
	}
	return ChunkReader::IsDataChunk(chunkHeader);
}

void PerformanceReader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	switch(chunkHeader.type)
	{
		case 2:
		{
			AccompanimentSettingsReader reader(this->accompanimentSettings, this->version);
			reader.ReadData(dataReader.InputStream());
		}
		break;
		case 4:
		case 32:
			this->unknownChunksAtBeginning.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
			break;
		case 10:
		{
			KeyboardSettingsReader reader(this->keyboardSettings, this->perfIndex);
			reader.ReadData(dataReader.InputStream());

			this->perfIndex++;
		}
		break;
		case 26:
		case 27:
		case 33:
			this->unknownChunksAtEnd.Push(UnknownChunk(chunkHeader, dataReader.InputStream()));
		break;
	}
}
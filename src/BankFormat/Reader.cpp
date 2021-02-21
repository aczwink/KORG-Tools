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
#include "Reader.hpp"
//Local
#include <libkorg/Sample.hpp>
#include <libkorg/EncryptedSample.hpp>
#include <libkorg/MultiSample.hpp>
#include <libkorg/SongBook.hpp>
#include <libkorg/SongBookEntry.hpp>
#include <libkorg/Sound.hpp>
#include <libkorg/Pad.hpp>
#include "libkorg/Style.hpp"
#include "libkorg/Performance.hpp"
#include "OC31Decompressor.hpp"
#include "TOCReader.hpp"
#include "../Reading/PerformanceReader.hpp"
#include "../Reading/StyleReader.hpp"
//Namespaces
using namespace BankFormat;
using namespace libKORG;

//Protected methods
String Reader::GetDebugDirName() const
{
	return u8"/home/amir/Desktop/korg/_OUT/_BANK/";
}

bool Reader::IsDataChunk(const ChunkHeader &chunkHeader)
{
	switch((ChunkType)chunkHeader.type)
	{
		case ChunkType::PerformancesData:
		case ChunkType::StyleObject:
			return true;
	}

	return ChunkReader::IsDataChunk(chunkHeader);
}

void Reader::ReadDataChunk(const ChunkHeader& chunkHeader, DataReader &dataReader)
{
	switch((ChunkType)chunkHeader.type)
	{
		case ChunkType::CrossReferenceTable:
			dataReader.Skip(chunkHeader.size);
			break;
		case ChunkType::KorgFile:
		{
			ASSERT_EQUALS(12_u32, chunkHeader.size);

			ASSERT_EQUALS(0x0B000000_u32, dataReader.ReadUInt32());
			ASSERT_EQUALS(0_u16, dataReader.ReadUInt16());
			ASSERT_EQUALS(0_u8, dataReader.ReadByte());
			ASSERT_EQUALS(FOURCC(u8"KORF"), this->ReadFourCC(dataReader.InputStream()));
			ASSERT_EQUALS(0_u8, dataReader.ReadByte());
		}
		break;
		case ChunkType::ObjectTOC:
		{
			TOCReader tocReader(chunkHeader, dataReader);
			this->headerEntries = tocReader.Read();
		}
		break;
		case ChunkType::PerformancesData:
		{
			/*{
				static int i = 0;
				FileOutputStream fileOutputStream(FileSystem::Path(u8"/home/amir/Desktop/korg/_OUT/perf"), true);
				dataReader.InputStream().FlushTo(fileOutputStream);
				exit(9);
			}*/

			PerformanceReader performanceReader;
			performanceReader.ReadData(dataReader.InputStream());

			this->AddObject(performanceReader.TakeSTSResult());
		}
		break;
		case ChunkType::StyleObject:
		{
			ASSERT_EQUALS(0, chunkHeader.version.major);
			ASSERT_EQUALS(0, chunkHeader.version.minor);

			/*{
				static int i = 0;
				FileOutputStream fileOutputStream(FileSystem::Path(u8"/home/amir/Desktop/korg/_OUT/style"), true);
				dataReader.InputStream().FlushTo(fileOutputStream);
				break;
			}*/

			StyleReader styleReader;
			styleReader.ReadData(dataReader.InputStream());
			Style* style = styleReader.TakeResult();

			this->AddObject(style);
		}
		break;
		default:
			stdOut << String::HexNumber(chunkHeader.id) << endl;
			NOT_IMPLEMENTED_ERROR;
	}
}

//Private methods
/*void Reader::ReadEntries(const DynamicArray<HeaderEntry>& headerEntries)
{
	for(const HeaderEntry& headerEntry : headerEntries)
	{
		ASSERT((chunkHeader.id == (uint32)ChunkId::MultiSampleData)
			|| (chunkHeader.id == (uint32)ChunkId::OldSoundDataMaybe)
			|| (chunkHeader.id == (uint32)ChunkId::PadData)
			|| (chunkHeader.id == (uint32)ChunkId::PCMData)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData02)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData03)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData10)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData20)
			|| (chunkHeader.id == (uint32)ChunkId::PerformanceData21)
			|| (chunkHeader.id == (uint32)ChunkId::SongBookListData)
			|| (chunkHeader.id == (uint32)ChunkId::SongBookListData1)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData00)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData0)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData1)
			|| (chunkHeader.id == (uint32)ChunkId::SoundData2), "???" + String::HexNumber(chunkHeader.id));

		if()
		{
			ASSERT_EQUALS(ObjectType::PCM, headerEntry.type);
		}

		InputStream& inputStream = chainedInputStream.GetEnd();
		BankObject* object;
		switch(headerEntry.type)
		{
			case ObjectType::MultiSample:
				object = new MultiSample(inputStream);
				break;
			case ObjectType::Performance:
			{
				PerformanceReader performanceReader;
				performanceReader.ReadData(inputStream);
				object = performanceReader.TakePerformanceResult();
			}
			break;
			case ObjectType::PAD:
				NOT_IMPLEMENTED_ERROR;
				//object = this->ReadInObect(new Pad, inputStream);
				break;
			case ObjectType::PCM:
				if(chunkHeader.flags & (uint8)ChunkHeaderFlags::Encrypted)
					object = new EncryptedSample(inputStream);
				else
					object = new Sample(inputStream);
				break;
			case ObjectType::SongBookEntry:
				object = new SongBookEntry(inputStream);
				break;
			case ObjectType::SongBook:
				object = new SongBook(inputStream);
				break;
			case ObjectType::Sound:
				object = new Sound(inputStream);
				break;
		}

		this->objectEntries.Push({ headerEntry.name, headerEntry.pos, object });

		//make sure we read all of it
		while(!inputStream.IsAtEnd())
		{
			uint32 nBytesSkipped = inputStream.Skip(c_io_blockSize);
			stdErr << nBytesSkipped << u8" bytes of trailing data found..." << endl;
		}
	}
}
*/
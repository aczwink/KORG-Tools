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
#include <StdXXTest.hpp>
#include <libkorg.hpp>
//Namespaces
using namespace libKORG;
using namespace libKORG::BankFormat;
using namespace StdXX;

TEST_SUITE(CompressionTests)
{
	void Compress(SeekableOutputStream& outputStream, const byte* data, uint32 dataLength)
	{
		Writer writer(outputStream);

		writer.WriteHeader();

		writer.BeginWritingIndex();
		HeaderEntry headerEntry;
		headerEntry.pos = 0;
		headerEntry.dataVersion = {0, 0};
		headerEntry.type = ObjectType::Sound;
		writer.WriteIndexEntry(headerEntry, ObjectStreamFormat::Compressed);
		writer.EndIndex();

		auto objectStream = writer.BeginWritingObjectData();
		objectStream->WriteBytes(data, dataLength);
		objectStream->Finalize();
		writer.EndWritingObject();

		writer.Finalize();
	}

	void Decompress(InputStream& inputStream, const byte* data, uint32 dataLength)
	{
		class DataChunkComparer : public ChunkReader
		{
		public:
			DynamicByteBuffer buffer;

		protected:
			ChunkReader *OnEnteringChunk(const ChunkHeader &chunkHeader) override
			{
				return this;
			}

			void ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader) override
			{
				switch((ChunkType)chunkHeader.type)
				{
					case ChunkType::KorgFile: //ignore
					case ChunkType::ObjectTOC:
					case ChunkType::CrossReferenceTable:
					{
						NullOutputStream nullOutputStream;
						dataReader.InputStream().FlushTo(nullOutputStream);
					}
					break;
					default:
						dataReader.InputStream().FlushTo(*buffer.CreateOutputStream());
				}
			}
		};

		DataChunkComparer comparer;
		comparer.ReadData(inputStream);

		ASSERT_EQUALS(0, MemCmp(comparer.buffer.Data(), data, dataLength));
	}

	TEST_CASE(SimpleTest)
	{
		DynamicByteBuffer buffer;

		String test = u8"Test";
		test.ToUTF8();
		Compress(*buffer.CreateOutputStream(), test.GetRawData(), test.GetSize());
		Decompress(*buffer.CreateInputStream(), test.GetRawData(), test.GetSize());
	}

	TEST_CASE(SimpleRedudancyTest)
	{
		DynamicByteBuffer buffer;

		String test = u8"TestTest";
		test.ToUTF8();
		Compress(*buffer.CreateOutputStream(), test.GetRawData(), test.GetSize());
		Decompress(*buffer.CreateInputStream(), test.GetRawData(), test.GetSize());
	}
}
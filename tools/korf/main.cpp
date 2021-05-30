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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

void DumpChunks(const Path& path, InputStream& inputStream);

class ChunkSerializer : public ChunkReader
{
public:
	//Constructor
	inline ChunkSerializer(UniquePointer<OutputStream>&& outputStream) : outputStream(Move(outputStream))
	{
	}

protected:
	//Methods
	ChunkReader* OnEnteringChunk(const ChunkHeader &chunkHeader) override
	{
		this->WriteChunkHeader(chunkHeader);

		return this;
	}

	void ReadDataChunk(const ChunkHeader &chunkHeader, DataReader &dataReader) override
	{
		this->WriteChunkHeader(chunkHeader);
		dataReader.InputStream().FlushTo(*this->outputStream);
	}

private:
	//Members
	UniquePointer<OutputStream> outputStream;

	//Inline
	inline void WriteChunkHeader(const ChunkHeader& chunkHeader)
	{
		DataWriter dataWriter(true, *this->outputStream);

		dataWriter.WriteUInt32(chunkHeader.id);
		dataWriter.WriteUInt32(chunkHeader.size);
	}
};

class ObjectTrackingReader : public BankFormat::Reader
{
public:
	//Members
	BinaryTreeMap<const BankFormat::HeaderEntry*, DynamicByteBuffer> objectsData;

protected:
	void ReadBankObject(BankFormat::ChunkType chunkType, const ChunkHeader &chunkHeader, const BankFormat::HeaderEntry &headerEntry, DataReader &dataReader) override
	{
		dataReader.InputStream().FlushTo(*this->objectsData[&headerEntry].CreateOutputStream());
	}

	ChunkReader* OnEnteringChunkedResourceChunk(const ChunkHeader &chunkHeader, const BankFormat::HeaderEntry &headerEntry) override
	{
		switch (BankFormat::ChunkType(chunkHeader.type))
		{
			case BankFormat::ChunkType::PerformancesData:
			case BankFormat::ChunkType::StyleObject:
				this->chunkSerializer = new ChunkSerializer(this->objectsData[&headerEntry].CreateOutputStream());
				return this->chunkSerializer.operator->();
		}
		return Reader::OnEnteringChunkedResourceChunk(chunkHeader, headerEntry);
	}

	void OnLeavingChunk(const ChunkHeader &chunkHeader) override
	{
		switch (BankFormat::ChunkType(chunkHeader.type))
		{
			case BankFormat::ChunkType::PerformancesData:
			case BankFormat::ChunkType::StyleObject:
				this->Next();
				break;
			default:
				Reader::OnLeavingChunk(chunkHeader);
		}
	}

private:
	//Members
	UniquePointer<ChunkSerializer> chunkSerializer;
};

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::SubCommandArgument commandGroup(u8"command", u8"The command to be executed");
	parser.AddPositionalArgument(commandGroup);

	CommandLine::UnsignedArgument inputPosArg(u8"input-pos", u8"Position of resource within the input korf file");

	CommandLine::Group dumpChunks(u8"dump-chunks", u8"Dump chunks into filesystem");
	dumpChunks.AddPositionalArgument(inputPosArg);
	commandGroup.AddCommand(dumpChunks);

	CommandLine::Group dumpObject(u8"dump-object", u8"Dump object into filesystem");
	dumpObject.AddPositionalArgument(inputPosArg);
	commandGroup.AddCommand(dumpObject);

	CommandLine::Group listContents(u8"list-contents", u8"List the contents of the input file");
	commandGroup.AddCommand(listContents);

	CommandLine::Group repackObject(u8"repack-object", u8"Repack a single object into a new bank file");
	repackObject.AddPositionalArgument(inputPosArg);
	commandGroup.AddCommand(repackObject);

	CommandLine::PathArgument inputPathArg(u8"input-path", u8"Path to the input korf file");
	parser.AddPositionalArgument(inputPathArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	Path inputPath = inputPathArg.Value(result);

	FileInputStream fileInputStream(inputPath);
	ObjectTrackingReader bankFormatReader;
	bankFormatReader.ReadData(fileInputStream);

	if(result.IsActivated(listContents))
	{
		stdOut << u8"Position: Type Version Name" << endl;
		for (const auto& kv: bankFormatReader.objectsData)
		{
			stdOut << String::Number(kv.key->pos, 10, 2) << u8": ";
			switch(kv.key->type)
			{
				case BankFormat::ObjectType::Performance:
					stdOut << u8"Performance";
					break;
				case BankFormat::ObjectType::Style:
					stdOut << u8"Style";
					break;
				case BankFormat::ObjectType::Sound:
					stdOut << u8"Sound";
					break;
				case BankFormat::ObjectType::MultiSample:
					stdOut << u8"MultiSamples";
					break;
				case BankFormat::ObjectType::PCM:
					stdOut << u8"PCM";
					break;
				case BankFormat::ObjectType::StylePerformances:
					stdOut << u8"Style performances";
					break;
				case BankFormat::ObjectType::PAD:
					stdOut << u8"Pad";
					break;
				case BankFormat::ObjectType::SongBookEntry:
					stdOut << u8"Songbook entry";
					break;
				case BankFormat::ObjectType::SongBook:
					stdOut << u8"Songbook";
					break;
			}

			stdOut << u8" " << kv.key->dataVersion.major << u8"." << kv.key->dataVersion.minor << u8" " << kv.key->name << endl;
		}
	}
	else
	{
		uint8 pos = inputPosArg.Value(result);

		for (const auto& kv: bankFormatReader.objectsData)
		{
			if(kv.key->pos == pos)
			{
				String name = String::Number((uint8)kv.key->type) + u8"_" + String::HexNumber(kv.key->dataVersion.AsUInt16(), 4) + u8"_" + kv.key->name;
				if(result.IsActivated(dumpChunks))
				{
					stdOut << u8"Dumping chunks of: " << name << endl;

					Path path = FileSystemsManager::Instance().OSFileSystem().GetWorkingDirectory() / name;
					DumpChunks(path, *kv.value.CreateInputStream());

					stdOut << endl;
				}
				else if(result.IsActivated(dumpObject))
				{
					kv.value.CreateInputStream()->FlushTo(stdOut);
				}
				else if(result.IsActivated(repackObject))
				{
					DynamicByteBuffer buffer;
					auto outputStream = buffer.CreateOutputStream();
					BankFormat::Writer writer(*outputStream);

					writer.WriteHeader();

					writer.BeginWritingIndex();
					writer.WriteIndexEntry(*kv.key);
					writer.EndIndex();

					auto& objectOutputStream = writer.BeginWritingObjectData();
					kv.value.CreateInputStream()->FlushTo(objectOutputStream);
					writer.EndWritingObject();

					writer.Finalize();

					buffer.CreateInputStream()->FlushTo(stdOut);
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
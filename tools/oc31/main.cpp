/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "../../src/BankFormat/OC31.hpp"
#include "../../src/BankFormat/OC31Compressor.hpp"
#include "../../src/BankFormat/OC31Decompressor.hpp"

using namespace libKORG;
using namespace StdXX;
using namespace StdXX::FileSystem;

enum class OC31Command
{
	Compress,
	Decompress,
	Dump
};

static void DumpOC31Chunks()
{
	auto& input = stdIn;

	DynamicArray<OC31Block> blocks = OC31ReadAllBlocks(input);

	CommonFileFormats::CSVWriter csvWriter(stdOut, CommonFileFormats::csvDialect_excel);
	csvWriter << u8"Block number" << u8"Type" << u8"Distance" << u8"Length" << u8"Extra" << u8"Block size" << u8"Uncompressed size of block" << u8"Compressed size" << u8"Uncompressed size" << endl;

	uint32 uncompressed = 0, blockCounter = 0, compressed = 0;
	for(const auto& block : blocks)
	{
		csvWriter << blockCounter;

		uint32 blockUncompressedSize;
		if(block.isBackreference)
		{
			csvWriter << u8"backref" << block.distance << block.length << block.nExtraBytes;
			blockUncompressedSize = block.length + block.nExtraBytes;
		}
		else
		{
			csvWriter << u8"raw" << u8"" << block.length << u8"";
			blockUncompressedSize = block.length;
		}

		uint32 blockSize = OC31ComputeOptimalBlockSize(block);

		uncompressed += blockUncompressedSize;
		compressed += blockSize;
		csvWriter << blockSize << blockUncompressedSize << compressed << uncompressed << endl;
		blockCounter++;
	}
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::EnumArgument<OC31Command> commandGroup(u8"command", u8"The command to be executed");
	parser.AddPositionalArgument(commandGroup);

	commandGroup.AddMapping(u8"compress", OC31Command::Compress, u8"Compress stdin using OC31 to stdout");
	commandGroup.AddMapping(u8"decompress", OC31Command::Decompress, u8"Decompress stdin using OC31 to stdout");
	commandGroup.AddMapping(u8"dump", OC31Command::Dump, u8"Dump OC31 chunks to stdout");

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();
	switch(commandGroup.Value(result))
	{
		case OC31Command::Compress:
		{
			DynamicByteBuffer inBuffer;
			{
				auto stream = inBuffer.CreateOutputStream();
				stdIn.FlushTo(*stream);
			}

			DynamicByteBuffer outBuffer;
			OC31Compressor compressor(outBuffer.CreateOutputStream());

			inBuffer.CreateInputStream()->FlushTo(compressor);
			compressor.Finalize();

			outBuffer.CreateInputStream()->FlushTo(stdOut);
		}
		break;
		case OC31Command::Decompress:
		{
			stdIn.Skip(4); //skip signature

			OC31Decompressor decompressor(stdIn);
			decompressor.FlushTo(stdOut);
		}
		break;
		case OC31Command::Dump:
			DumpOC31Chunks();
		break;
	}

	return EXIT_SUCCESS;
}
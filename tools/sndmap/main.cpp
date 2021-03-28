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

static DynamicArray<Tuple<String, ProgramChangeSequence>> ReadXMLSoundMap(const FileSystem::Path& path)
{
	FileInputStream fileInputStream(path);
	BufferedInputStream bufferedInputStream(fileInputStream);
	Serialization::XmlDeserializer deserializer(bufferedInputStream);

	DynamicArray<Tuple<String, ProgramChangeSequence>> result;

	deserializer.EnterElement(u8"SoundMap");
	while(deserializer.MoreChildrenExistsAtCurrentLevel())
	{
		deserializer.EnterElement(u8"SoundSet");
		while(deserializer.MoreChildrenExistsAtCurrentLevel())
		{
			deserializer.EnterElement(u8"Bank");
			while(deserializer.MoreChildrenExistsAtCurrentLevel())
			{
				deserializer.EnterElement(u8"SoundEntry");

				deserializer.EnterAttributes();

				String name;
				deserializer >> Serialization::Binding(u8"name", name);
				uint8 msb, lsb, pc;
				uint8 soundSetType = static_cast<uint8>(SoundSetType::KORG);

				deserializer >> Serialization::Binding(u8"bankSelectMSB", msb);
				deserializer >> Serialization::Binding(u8"bankSelectLSB", lsb);
				deserializer >> Serialization::Binding(u8"programChange", pc);
				if(deserializer.HasAttribute(u8"soundSet"))
					deserializer >> Serialization::Binding(u8"soundSet", soundSetType);

				deserializer.LeaveAttributes();

				result.Push({ name, ProgramChangeSequence(static_cast<SoundSetType>(soundSetType), msb, lsb, pc) });

				deserializer.LeaveElement();
			}
			deserializer.LeaveElement();
		}
		deserializer.LeaveElement();
	}
	deserializer.LeaveElement();

	return result;
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument factorySoundMapPathArg(u8"factory-sound-map-path", u8"Path to the factory sound map");
	parser.AddPositionalArgument(factorySoundMapPathArg);

	CommandLine::PathArgument sourceSetPathArg(u8"source-set-path", u8"Path to the source set");
	parser.AddPositionalArgument(sourceSetPathArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	FileSystem::Path factorySoundMapPath = factorySoundMapPathArg.Value(result);
	FileSystem::Path sourceSetPath = sourceSetPathArg.Value(result);

	Set set(sourceSetPath);

	DynamicArray<Tuple<String, ProgramChangeSequence>> soundMap = ReadXMLSoundMap(factorySoundMapPath);

	for(const auto& bank : set.SoundBanks())
	{
		uint8 msb = 121 - bank.key.IsDrumKit();
		uint8 lsb = 64 + bank.key.UserNumber();

		for(const auto& soundEntry : bank.value.Objects())
		{
			soundMap.Push({ soundEntry.value.Get<0>(), ProgramChangeSequence(msb, lsb, soundEntry.key) });
		}
	}

	CommonFileFormats::CSVWriter csvWriter(stdOut, CommonFileFormats::csvDialect_excel);
	csvWriter << u8"Sound name" << u8"Program change sequence" << u8"Sound set" << endl;
	for(const auto& tuple : soundMap)
	{
		csvWriter << tuple.Get<0>()
		        << tuple.Get<1>().ToString()
		        << (uint32)tuple.Get<1>().SoundSetType()
		        << endl;
	}

	return EXIT_SUCCESS;
}
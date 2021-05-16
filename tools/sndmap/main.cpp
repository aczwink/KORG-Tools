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

static DynamicArray<Tuple<String, String, ProgramChangeSequence>> ReadXMLSoundMap(const FileSystem::Path& path)
{
	FileInputStream fileInputStream(path);
	BufferedInputStream bufferedInputStream(fileInputStream);
	Serialization::XmlDeserializer deserializer(bufferedInputStream);

	DynamicArray<Tuple<String, String, ProgramChangeSequence>> result;

	deserializer.EnterElement(u8"SoundMap");
	while(deserializer.MoreChildrenExistsAtCurrentLevel())
	{
		deserializer.EnterElement(u8"SoundSet");

		deserializer.EnterAttributes();
		String setName;
		deserializer >> Serialization::Binding(u8"name", setName);
		deserializer.LeaveAttributes();

		while(deserializer.MoreChildrenExistsAtCurrentLevel())
		{
			deserializer.EnterElement(u8"Bank");

			deserializer.EnterAttributes();
			String bankName;
			deserializer >> Serialization::Binding(u8"name", bankName);
			deserializer.LeaveAttributes();

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

				result.Push({ setName + u8"/" + bankName, name, ProgramChangeSequence(static_cast<SoundSetType>(soundSetType), msb, lsb, pc) });

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

	DynamicArray<Tuple<String, String, ProgramChangeSequence>> soundMap = ReadXMLSoundMap(factorySoundMapPath);

	for(const auto& bank : set.SoundBanks())
	{
		for(const auto& soundEntry : bank.value.Objects())
		{
			String bankName = u8"User";
			bankName += bank.key.IsDrumKit() ? u8" DK" : u8"";
			soundMap.Push({ bankName, soundEntry.value.Get<0>(), Set::CreateRAMSoundProgramChangeSequence(bank.key, soundEntry.) });
		}
	}

	CommonFileFormats::CSVWriter csvWriter(stdOut, CommonFileFormats::csvDialect_excel);
	csvWriter << u8"Bank name" << u8"Sound name" << u8"Program change sequence" << u8"Sound set" << endl;
	for(const auto& tuple : soundMap)
	{
		csvWriter << tuple.Get<0>()
		        << tuple.Get<1>()
		        << tuple.Get<2>().ToString()
		        << (uint32)tuple.Get<2>().SoundSetType()
		        << endl;
	}

	return EXIT_SUCCESS;
}
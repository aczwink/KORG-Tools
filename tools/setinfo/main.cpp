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
#include <libkorg.hpp>
#include "HumanReadableOutputter.hpp"
#include "StyleOutputter.hpp"
#include "PerformanceOutputter.hpp"
#include "XMLOutputter.hpp"
#include "SoundOutputter.hpp"
#include "MultiSamplesOutputter.hpp"
#include "SampleOutputter.hpp"

using namespace libKORG;

struct PrintSettings
{
	bool showObjects;
	Optional<uint8> bankNumber;
	Optional<uint8> posNumber;
};

template<typename BankNumberType, typename BankObjectType, typename ObjectOutputter>
void PrintBanks(const PrintSettings& printSettings, const BankCollection<BankNumberType, BankObjectType>& banks, FormattedOutputter& outputter)
{
	uint8 bankNumber = 0;
	for(const auto& bankEntry : banks.Entries())
	{
		if(printSettings.bankNumber.HasValue() && (*printSettings.bankNumber != bankNumber++))
			continue;

		Section bankSection(bankEntry.bankNumber.ToString(), outputter);

		uint8 posNumber = 0;
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(printSettings.posNumber.HasValue() && (*printSettings.posNumber != posNumber++))
				continue;

			Section objectSection(BankPositionToString(objectEntry.pos) + u8" - " + objectEntry.name, outputter);

			if(printSettings.showObjects)
			{
				ObjectOutputter objectOutputter(outputter);
				objectOutputter.Output(*objectEntry.object);
			}
		}
	}
}

void PrintMultiSamples(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	MultiSamplesOutputter multiSamplesOutputter(outputter);
	multiSamplesOutputter.Output(set.MultiSamples());
}

void PrintPerformanceBanks(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	//PrintBanks<PerformanceBankNumber, PerformanceBank, PerformanceOutputter>(printSettings, set.PerformanceBanks(), outputter); //PerformanceBankNumberToString
}

void PrintSampleBanks(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	PrintBanks<SampleBankNumber, AbstractSample, SampleOutputter>(printSettings, set.sampleBanks, outputter);
}

void PrintSoundBanks(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	PrintBanks<SoundBankNumber, SoundObject, SoundOutputter>(printSettings, set.soundBanks, outputter);
}

void PrintStyleBanks(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	//PrintBanks<StyleBankNumber, StyleBank, StyleOutputter>(printSettings, set.StyleBanks(), outputter); //StyleBankNumberToString
}

enum class ResourceType
{
	MultiSamples,
	Performances,
	Samples,
	Sounds,
	Styles
};

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument inputPathArg(u8"input-path", u8"Path to the set that should be dumped");
	parser.AddPositionalArgument(inputPathArg);

	CommandLine::EnumArgument<ResourceType> typeArgument(u8"type", u8"The resource type of the set that should be dumped");
	typeArgument.AddMapping(u8"multisamples", ResourceType::MultiSamples, u8"Dump multisamples");
	typeArgument.AddMapping(u8"performances", ResourceType::Performances, u8"Dump performances");
	typeArgument.AddMapping(u8"samples", ResourceType::Samples, u8"Dump samples");
	typeArgument.AddMapping(u8"sounds", ResourceType::Sounds, u8"Dump sounds");
	typeArgument.AddMapping(u8"styles", ResourceType::Styles, u8"Dump styles");
	parser.AddPositionalArgument(typeArgument);

	CommandLine::OptionWithArgument bankOpt(u8'b', u8"bank-number", u8"Bank number to generate info for");
	parser.AddOption(bankOpt);

	CommandLine::OptionWithArgument inputPosOpt(u8'p', u8"input-pos", u8"Position of resource within the bank(s)");
	parser.AddOption(inputPosOpt);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	FileSystem::Path setPath = inputPathArg.Value(result);
	Set set(setPath);

	ResourceType resourceType = typeArgument.Value(result);

	PrintSettings printSettings;
	printSettings.showObjects = true;

	if(result.IsActivated(bankOpt))
	{
		printSettings.bankNumber = result.Value(bankOpt).ToUInt();
	}
	if(result.IsActivated(inputPosOpt))
	{
		printSettings.posNumber = result.Value(inputPosOpt).ToUInt();
	}

	UniquePointer<FormattedOutputter> outputter = new XMLOutputter(stdOut);

	switch(resourceType)
	{
		case ResourceType::MultiSamples:
			PrintMultiSamples(printSettings, set, *outputter);
			break;
		case ResourceType::Performances:
			PrintPerformanceBanks(printSettings, set, *outputter);
			break;
		case ResourceType::Samples:
			PrintSampleBanks(printSettings, set, *outputter);
			break;
		case ResourceType::Sounds:
			PrintSoundBanks(printSettings, set, *outputter);
			break;
		case ResourceType::Styles:
			PrintStyleBanks(printSettings, set, *outputter);
			break;
	}

	return EXIT_SUCCESS;
}

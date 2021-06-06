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
#include "PerformanceV1Outputter.hpp"
#include "XMLOutputter.hpp"
#include "SoundOutputter.hpp"
#include "MultiSamplesOutputter.hpp"
#include "SampleOutputter.hpp"
#include "PerformanceOutputter.hpp"

using namespace libKORG;

struct PrintSettings
{
	bool showObjects;
	Optional<String> bankName;
	Optional<uint8> posNumber;
};

template<typename BankNumberType, typename BankObjectType, typename ObjectOutputter>
void PrintBanks(const PrintSettings& printSettings, const BankCollection<BankNumberType, BankObjectType>& banks, FormattedOutputter& outputter)
{
	uint8 bankNumber;
	if(printSettings.bankName.HasValue())
		bankNumber = BankNumberType::FromBankName(*printSettings.bankName).Number();

	for(const auto& bankEntry : banks.Entries())
	{
		if(printSettings.bankName.HasValue() && (bankNumber != bankEntry.bankNumber.Number()))
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

void PrintMultiSamples(const PrintSettings& printSettings, const Set& set, FormattedOutputter& outputter)
{
	MultiSamplesOutputter multiSamplesOutputter(outputter);
	multiSamplesOutputter.Output(set.MultiSamples());
}

void PrintPadBanks(const PrintSettings& printSettings, const Set& set, FormattedOutputter& outputter)
{
	PrintBanks<PadBankNumber, StyleObject, StyleOutputter>(printSettings, set.padBanks, outputter);
}

void PrintPerformanceBanks(const PrintSettings& printSettings, const Set& set, FormattedOutputter& outputter)
{
	PrintBanks<PerformanceBankNumber, PerformanceObject, PerformanceOutputter>(printSettings, set.performanceBanks, outputter); //PerformanceBankNumberToString
}

void PrintSampleBanks(const PrintSettings& printSettings, const Set& set, FormattedOutputter& outputter)
{
	PrintBanks<SampleBankNumber, AbstractSample, SampleOutputter>(printSettings, set.sampleBanks, outputter);
}

void PrintSoundBanks(const PrintSettings& printSettings, const Set& set, FormattedOutputter& outputter)
{
	PrintBanks<SoundBankNumber, SoundObject, SoundOutputter>(printSettings, set.soundBanks, outputter);
}

void PrintStyleBanks(const PrintSettings& printSettings, const Set& set, FormattedOutputter& outputter)
{
	PrintBanks<StyleBankNumber, FullStyle, StyleOutputter>(printSettings, set.styleBanks, outputter); //StyleBankNumberToString
}

enum class ResourceType
{
	MultiSamples,
	Pads,
	Performances,
	Samples,
	Sounds,
	Styles
};

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::EnumArgument<ResourceType> typeArgument(u8"type", u8"The resource type of the sourceSet that should be dumped");
	typeArgument.AddMapping(u8"multisamples", ResourceType::MultiSamples, u8"Dump multisamples");
	typeArgument.AddMapping(u8"pads", ResourceType::Pads, u8"Dump pads");
	typeArgument.AddMapping(u8"performances", ResourceType::Performances, u8"Dump performances");
	typeArgument.AddMapping(u8"samples", ResourceType::Samples, u8"Dump samples");
	typeArgument.AddMapping(u8"sounds", ResourceType::Sounds, u8"Dump sounds");
	typeArgument.AddMapping(u8"styles", ResourceType::Styles, u8"Dump styles");
	parser.AddPositionalArgument(typeArgument);

	CommandLine::PathArgument inputPathArg(u8"input-path", u8"Path to the sourceSet that should be dumped");
	parser.AddPositionalArgument(inputPathArg);

	CommandLine::OptionWithArgument bankOpt(u8'b', u8"bank", u8"Bank name to generate info for");
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
	printSettings.showObjects = result.IsActivated(inputPosOpt);

	if(result.IsActivated(bankOpt))
	{
		printSettings.bankName = result.Value(bankOpt);
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
		case ResourceType::Pads:
			PrintPadBanks(printSettings, set, *outputter);
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

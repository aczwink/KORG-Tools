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
#include "ResourceImporter.hpp"

enum class ResourceType
{
	Performance
};

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::OptionWithArgument soundLocationOpt(u8's', u8"sound-location", u8"Preferred base location where to insert new sounds. Format is: Bank,Position");
	parser.AddOption(soundLocationOpt);

	CommandLine::OptionWithArgument performanceLocationOpt(u8'p', u8"performance-location", u8"Preferred base location where to insert new performances. Format is: Bank,Position");
	parser.AddOption(performanceLocationOpt);

	CommandLine::PathArgument inputSetPathArg(u8"input-set-path", u8"Path to the set that holds the resource to be imported");
	parser.AddPositionalArgument(inputSetPathArg);

	CommandLine::EnumArgument<ResourceType> typeArgument(u8"type", u8"The resource type of the set that should be imported");
	typeArgument.AddMapping(u8"performance", ResourceType::Performance, u8"Import performance");
	parser.AddPositionalArgument(typeArgument);

	CommandLine::StringArgument inputBankArg(u8"input-bank", u8"Bank of the resource that should be imported");
	parser.AddPositionalArgument(inputBankArg);

	CommandLine::UnsignedArgument inputPosArg(u8"input-pos", u8"Position within the bank of the resource that should be imported");
	parser.AddPositionalArgument(inputPosArg);

	CommandLine::PathArgument targetSetPathArg(u8"target-set-path", u8"Path to the set that the resource is imported to");
	parser.AddPositionalArgument(targetSetPathArg);

	CommandLine::StringArgument targetModelArg(u8"target-model", u8"The model of the target set");
	parser.AddPositionalArgument(targetModelArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	const Model* targetModel = FindModel(targetModelArg.Value(result));
	if(targetModel == nullptr)
	{
		stdErr << u8"Unknown model: " << targetModelArg.Value(result) << endl;
		return EXIT_FAILURE;
	}

	Config config;
	if(result.IsActivated(soundLocationOpt))
	{
		auto parts = soundLocationOpt.Value(result).Split(u8",");
		config.soundInsertSlot = {SoundBankNumber::FromBankName(parts[0]), (uint8)parts[1].ToUInt32()};
	}
	if(result.IsActivated(performanceLocationOpt))
	{
		auto parts = performanceLocationOpt.Value(result).Split(u8",");
		config.performanceInsertSlot = {PerformanceBankNumber::FromBankName(parts[0]), (uint8)parts[1].ToUInt32()};
	}

	Set sourceSet(inputSetPathArg.Value(result));
	IdsCorrector idsCorrector(sourceSet);
	idsCorrector.Correct();
	if(idsCorrector.ErrorsDetected())
	{
		stdErr << u8"The source set is erroneous." << endl
			<< idsCorrector.Errors().missingSamplesCount << u8" samples are missing (either metadata or payload)." << endl
			<< idsCorrector.Errors().missingDrumSamplesCount << u8" drum samples (metadata) are missing." << endl;
	}

	Set targetSet(targetSetPathArg.Value(result), *targetModel);
	ResourceImporter resourceImporter(sourceSet, targetSet, config);
	switch(typeArgument.Value(result))
	{
		case ResourceType::Performance:
		{
			PerformanceBankNumber bankNumber = PerformanceBankNumber::FromBankName(inputBankArg.Value(result));
			resourceImporter.ImportPerformance(bankNumber, inputPosArg.Value(result));
		}
		break;
	}

	return EXIT_SUCCESS;
}
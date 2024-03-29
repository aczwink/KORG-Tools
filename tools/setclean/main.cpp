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
//Local
#include "SetCleaner.hpp"

enum class ResourceType
{
	DrumSamples,
	MultiSamples,
	Samples,
	Sounds,
};

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::Option allOpt(u8'a', u8"all", u8"Show also referenced resources");
	parser.AddOption(allOpt);

	CommandLine::Option ignoreSTSOpt(u8'i', u8"ignore-sts", u8"If specified, STS are ignored, i.e. their assigned sounds are not considered");
	parser.AddOption(ignoreSTSOpt);

	CommandLine::Option ignorePerformanceAccSettingsOpt(u8'I', u8"ignore-performance-acc-settings", u8"If specified, accompaniment sounds for performances are ignore, i.e. their assigned sounds are not considered");
	parser.AddOption(ignorePerformanceAccSettingsOpt);

	CommandLine::EnumArgument<ResourceType> typeArgument(u8"type", u8"The resource type of the set that should be cleaned");
	typeArgument.AddMapping(u8"drumsamples", ResourceType::DrumSamples, u8"Remove unused drum samples");
	typeArgument.AddMapping(u8"multisamples", ResourceType::MultiSamples, u8"Remove unused multisamples");
	typeArgument.AddMapping(u8"samples", ResourceType::Samples, u8"Remove unused samples");
	typeArgument.AddMapping(u8"sounds", ResourceType::Sounds, u8"Remove unused sounds");
	parser.AddPositionalArgument(typeArgument);

	CommandLine::PathArgument inputSetPathArg(u8"set-path", u8"Path to the set that should be cleaned");
	parser.AddPositionalArgument(inputSetPathArg);

	CommandLine::StringArgument modelArg(u8"model", u8"The model for the set");
	parser.AddPositionalArgument(modelArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	const Model* model = FindModel(modelArg.Value(result));
	if(model == nullptr)
	{
		stdErr << u8"Unknown model: " << modelArg.Value(result) << endl;
		return EXIT_FAILURE;
	}

	Set set(inputSetPathArg.Value(result), *model);

	IdsCorrector idsCorrector(set);
	idsCorrector.Correct();
	if(idsCorrector.ErrorsDetected())
	{
		stdErr << u8"The source set is erroneous." << endl
			   << idsCorrector.Errors().missingSamplesCount << u8" samples are missing (either metadata or payload)." << endl
			   << idsCorrector.Errors().missingDrumSamplesCount << u8" drum samples (metadata) are missing." << endl;
	}

	bool showAll = result.IsActivated(allOpt);

	SetCleaner setCleaner(set);
	switch(typeArgument.Value(result))
	{
		case ResourceType::DrumSamples:
			setCleaner.RemoveUnusedDrumSamples();
			break;
		case ResourceType::MultiSamples:
			setCleaner.RemoveUnusedMultiSamples();
			break;
		case ResourceType::Samples:
			setCleaner.RemoveUnusedSamples(showAll);
			break;
		case ResourceType::Sounds:
			setCleaner.RemoveUnusedSounds(result.IsActivated(ignorePerformanceAccSettingsOpt), result.IsActivated(ignoreSTSOpt), showAll);
			break;
	}

	return EXIT_SUCCESS;
}
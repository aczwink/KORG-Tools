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
	MultiSamples,
	Samples,
	Sounds,
};

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::EnumArgument<ResourceType> typeArgument(u8"type", u8"The resource type of the set that should be cleaned");
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

	SetCleaner setCleaner(inputSetPathArg.Value(result), *model);
	switch(typeArgument.Value(result))
	{
		case ResourceType::MultiSamples:
			setCleaner.RemoveUnusedMultiSamples();
			break;
		case ResourceType::Samples:
			setCleaner.RemoveUnusedSamples();
			break;
		case ResourceType::Sounds:
			setCleaner.RemoveUnusedSounds();
			break;
	}

	return EXIT_SUCCESS;
}
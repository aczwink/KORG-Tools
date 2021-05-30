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
#include "SetConverter.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument inputSetPathArg(u8"input-sourceSet-path", u8"Path to the sourceSet that should be converted");
	parser.AddPositionalArgument(inputSetPathArg);

	CommandLine::PathArgument targetSetPathArg(u8"target-sourceSet-path", u8"Path to the converted sourceSet. The directory must not exist");
	parser.AddPositionalArgument(targetSetPathArg);

	CommandLine::StringArgument modelArg(u8"target-model", u8"The target model for the sourceSet to be converted");
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

	SetConverter setConverter(inputSetPathArg.Value(result), targetSetPathArg.Value(result), *model);
	setConverter.Convert();

	return EXIT_SUCCESS;
}
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

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument setPathArg(u8"set-path", u8"Path to the set");
	parser.AddPositionalArgument(setPathArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	Path setPath = setPathArg.Value(result);

	Set set(setPath);
	for(const auto& bank : set.SampleBanks())
	{
		for(const auto& entry : bank.value.Objects())
		{
			const String& sampleName = entry.value.Get<0>();
			const AbstractSample& sample = *entry.value.Get<1>();

			if(IS_INSTANCE_OF(&sample, EncryptedSample))
			{
				stdOut << u8"Skipping encrypted sample: " << sampleName << endl;
			}
			else
			{
				stdOut << u8"Exporting sample: " << sampleName << endl;
			}
		}
	}

	return EXIT_SUCCESS;
}
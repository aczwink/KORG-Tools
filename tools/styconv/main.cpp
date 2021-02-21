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
using namespace libKORG;

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument sourceSetPathArg(u8"source-set-path", u8"Path to the source set");
	parser.AddPositionalArgument(sourceSetPathArg);

	CommandLine::StringArgument sourceBankArg(u8"source-bank", u8"Source bank name");
	parser.AddPositionalArgument(sourceBankArg);

	CommandLine::UnsignedArgument sourcePosNumberArg(u8"source-pos", u8"Source position in bank");
	parser.AddPositionalArgument(sourcePosNumberArg);

	CommandLine::PathArgument targetSetPathArg(u8"target-set-path", u8"Path to the target set");
	parser.AddPositionalArgument(targetSetPathArg);

	CommandLine::StringArgument targetBankArg(u8"target-bank", u8"Target bank name");
	parser.AddPositionalArgument(targetBankArg);

	CommandLine::UnsignedArgument targetPosNumberArg(u8"target-pos", u8"Target position in bank");
	parser.AddPositionalArgument(targetPosNumberArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	struct
	{
		FileSystem::Path setPath;
		uint8 bankNumber;
		uint8 posNumber;
	} sourceInfo;

	sourceInfo.setPath = sourceSetPathArg.Value(result);
	sourceInfo.bankNumber = ParseStyleBankName(sourceBankArg.Value(result));
	sourceInfo.posNumber = sourcePosNumberArg.Value(result);

	struct
	{
		FileSystem::Path setPath;
		uint8 bankNumber;
		uint8 posNumber;
	} targetInfo;

	targetInfo.setPath = targetSetPathArg.Value(result);
	targetInfo.bankNumber = ParseStyleBankName(targetBankArg.Value(result));
	sourceInfo.posNumber = targetPosNumberArg.Value(result);




	Set sourceSet(sourceInfo.setPath);

	auto& entry = sourceSet.StyleBanks()[sourceInfo.bankNumber].Objects()[sourceInfo.posNumber];
	const String& styleName = entry.Get<0>();

	SharedPointer<FullStyle> mappedStyle = new FullStyle(*entry.Get<1>());

	if(!FileSystem::File(targetInfo.setPath).Exists())
		Set::Create(targetInfo.setPath);

	Set targetSet(targetInfo.setPath);
	if(!targetSet.StyleBanks().Contains(targetInfo.bankNumber))
		targetSet.StyleBanks().Insert(targetInfo.bankNumber, {});

	targetSet.StyleBanks()[targetInfo.bankNumber].AddObject(styleName, targetInfo.posNumber, mappedStyle);
	targetSet.Save();

	return EXIT_SUCCESS;
}

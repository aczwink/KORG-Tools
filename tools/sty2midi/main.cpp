/*
 * Copyright (c) 2024-2026 Amir Czwink (amir130@hotmail.de)
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

static void ExportVariation(uint8 bpm, const Math::Rational<uint8>& timeSig, uint8 variation, const IStyleElementView& variationView, const Style::StyleData& styleData)
{
	StandardMIDIFormatConverter converter(bpm, timeSig, false);

	uint8 chordVariationsCount = variationView.GetNumberOfChordVariations();
	for(uint8 i = 0; i < chordVariationsCount; i++)
	{
		if(!variationView.GetChordVariation(i).DoesHaveAnyData())
			continue;

		auto program = converter.LoadVariation(variation, i, styleData);

		FileOutputStream fileOutputStream(u8"V" + String::Number(variation+1) + u8"-CV" + String::Number(i+1) + u8".mid");
		program.Write(fileOutputStream);
	}
}

static void ExportToMIDI(const FileSystem::Path &setPath, const StyleBankNumber &bankNumber, uint8 pos)
{
	Set set(setPath);

	const FullStyle& fullStyle = set.styleBanks[bankNumber].GetObject(pos);
	const Style::StyleData& styleData = fullStyle.Style().data;
	uint8 bpm = fullStyle.STS().MetronomeTempo();

	StyleView styleView(styleData);
	auto timeSig = styleView.TimeSignature();

	for(uint8 i = 0; i < 4; i++)
	{
		if(!styleView.IsVariationDataAvailable(i) || !styleView.IsVariationEnabled(i))
			continue;

		ExportVariation(bpm, timeSig, i, styleView.GetVariation(i), styleData);
	}
	//TODO: other style elements
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument inputPathArg(u8"input-path", u8"Path to the input set");
	parser.AddPositionalArgument(inputPathArg);

	CommandLine::StringArgument bankArg(u8"bank", u8"Bank name");
	parser.AddPositionalArgument(bankArg);

	CommandLine::UnsignedArgument inputPosArg(u8"input-pos", u8"Position of the style within the bank");
	parser.AddPositionalArgument(inputPosArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	ExportToMIDI(inputPathArg.Value(result), StyleBankNumber::FromBankName(bankArg.Value(result)), static_cast<uint8>(inputPosArg.Value(result)));

	return EXIT_SUCCESS;
}
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
using namespace StdXX;

struct BankSelection
{
	FileSystem::Path setPath;
	StyleBankNumber bankNumber = 0;
	uint8 posOffset;
};

struct BankSelectionWithModel : BankSelection
{
	const Model* model;
};

UniquePointer<SingleTouchSettings> MapSTS(const SingleTouchSettings& sts, const Model* model)
{
	if(model && (sts.Version() > model->GetSupportedResourceVersions().maxPerformanceVersion.major))
	{
		PerformanceV2ToV1Converter converter;
		return new SingleTouchSettings(converter.Convert(sts));
	}

	return new SingleTouchSettings(sts);
}

void ConvertStyleBank(const BankSelection& source, const BankSelectionWithModel& target, const SoundMapper& soundMapper)
{
	stdOut << u8"Loading source set..." << endl;
	Set sourceSet(source.setPath);

	stdOut << u8"Loading target set..." << endl;
	if(!FileSystem::File(target.setPath).Exists())
		Set::Create(target.setPath, *target.model);
	Set targetSet(target.setPath, *target.model);

	const auto& entries = sourceSet.styleBanks[source.bankNumber].Objects();
	auto& targetStyleBank = targetSet.styleBanks[target.bankNumber];
	for(const auto& entry : entries)
	{
		const String& styleName = entry.name;
		const FullStyle& fullStyle = *entry.object;

		if(source.posOffset > entry.pos)
			continue;
		uint8 targetPos = entry.pos - source.posOffset;
		if(target.model and (targetPos >= target.model->GetBankSetup().styleBanks.nStylesPerBank))
		{
			stdOut << u8"Skipping style '" << styleName << u8"' because bank can't hold that many styles" << endl;
			continue;
		}

		UniquePointer<StyleObject> mappedStyle = new StyleObject(fullStyle.Style());
		UniquePointer<SingleTouchSettings> mappedSTS = MapSTS(fullStyle.STS(), target.model);

		soundMapper.Map(*mappedStyle);
		soundMapper.Map(*mappedSTS);

		SharedPointer<FullStyle> mappedFullStyle = new FullStyle(Move(mappedStyle), Move(mappedSTS));

		targetStyleBank.AddObject(styleName, targetPos, mappedFullStyle);
	}
	targetSet.Save();
}

static BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence> LoadSoundMapping(const String& pathString)
{
	FileSystem::Path inputPath = FileSystem::FileSystemsManager::Instance().OSFileSystem().FromNativePath(pathString);
	FileInputStream fileInputStream(inputPath);
	BufferedInputStream bufferedInputStream(fileInputStream);
	TextReader textReader(bufferedInputStream, TextCodecType::UTF8);

	CommonFileFormats::CSVReader csvReader(textReader, CommonFileFormats::csvDialect_excel);
	BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence> map;

	csvReader.ReadRow(); //skip column headers

	String tmp;
	while(!textReader.IsAtEnd())
	{
		csvReader.ReadCell(tmp);
		csvReader.ReadCell(tmp);

		csvReader.ReadCell(tmp);
		auto parts = tmp.Split(u8".");
		csvReader.ReadCell(tmp);

		ProgramChangeSequence source(static_cast<SoundSetType>(tmp.ToUInt32()), parts[0].ToUInt32(), parts[1].ToUInt32(), parts[2].ToUInt32());

		csvReader.ReadCell(tmp);
		csvReader.ReadCell(tmp);

		csvReader.ReadCell(tmp);
		parts = tmp.Split(u8".");
		csvReader.ReadCell(tmp);

		ProgramChangeSequence target(static_cast<SoundSetType>(tmp.ToUInt32()), parts[0].ToUInt32(), parts[1].ToUInt32(), parts[2].ToUInt32());

		map[source] = target;
	}

	return map;
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument sourceSetPathArg(u8"source-sourceSet-path", u8"Path to the source sourceSet");
	parser.AddPositionalArgument(sourceSetPathArg);

	CommandLine::StringArgument sourceBankArg(u8"source-bank", u8"Source bank name");
	parser.AddPositionalArgument(sourceBankArg);

	CommandLine::PathArgument targetSetPathArg(u8"target-sourceSet-path", u8"Path to the target sourceSet");
	parser.AddPositionalArgument(targetSetPathArg);

	CommandLine::StringArgument targetBankArg(u8"target-bank", u8"Target bank name");
	parser.AddPositionalArgument(targetBankArg);

	CommandLine::OptionWithArgument targetModelOpt(u8'm', u8"target-model", u8"The model that the target sourceSet is for");
	parser.AddOption(targetModelOpt);

	CommandLine::OptionWithArgument offsetOpt(u8'o', u8"offset", u8"The first position number in the bank that should be converted. Lower numbers are skipped.");
	parser.AddOption(offsetOpt);

	CommandLine::OptionWithArgument soundMappingPathOpt(u8's', u8"sound-mapping", u8"A path to a sound mapping csv file that is used to remap sounds");
	parser.AddOption(soundMappingPathOpt);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	BankSelection source;
	source.setPath = sourceSetPathArg.Value(result);
	source.bankNumber = StyleBankNumber::FromBankName(sourceBankArg.Value(result));
	source.posOffset = result.IsActivated(offsetOpt) ? result.Value(offsetOpt).ToUInt32() : 0;

	BankSelectionWithModel target;
	target.setPath = targetSetPathArg.Value(result);
	target.bankNumber = StyleBankNumber::FromBankName(targetBankArg.Value(result));
	target.model = nullptr;
	if(result.IsActivated(targetModelOpt))
	{
		target.model = FindModel(result.Value(targetModelOpt));
		if(target.model == nullptr)
		{
			stdErr << u8"UnknownMaster model: " << result.Value(targetModelOpt) << endl;
			return EXIT_FAILURE;
		}
	}

	BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence> soundMappings = result.IsActivated(soundMappingPathOpt) ? LoadSoundMapping(result.Value(soundMappingPathOpt))
																 : BinaryTreeMap<ProgramChangeSequence, ProgramChangeSequence>();
	SoundMapper soundMapper(Move(soundMappings));

	ConvertStyleBank(source, target, soundMapper);

	return EXIT_SUCCESS;
}
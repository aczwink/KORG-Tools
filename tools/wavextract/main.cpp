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
using namespace libKORG::Sample;
using namespace StdXX;
using namespace StdXX::Multimedia;

Packet* EncodeAudio(Frame& audioFrame, CodingFormatId targetFormatId, Stream& stream)
{
	UniquePointer<EncoderContext> encoderContext = CodingFormat::GetCodingFormatById(targetFormatId)->GetBestMatchingEncoder()->CreateContext(stream);
	encoderContext->Encode(audioFrame);

	return encoderContext->GetNextPacket();
}

void ExportWave(const FileSystem::Path& outPath, const SampleData& sampleData, const MultiSamples::SampleEntry* multiSamplesSampleEntry)
{
	FileOutputStream file(outPath.String() + u8".wav");

	const Format* format = Format::FindByExtension(u8"wav");
	UniquePointer<Muxer> muxer = format->CreateMuxer(file);

	AudioStream* targetStream = new AudioStream;
	muxer->AddStream(targetStream);

	targetStream->SetCodingFormat(CodingFormatId::PCM_S16LE);
	targetStream->sampleFormat = AudioSampleFormat(1, AudioSampleType::S16, false);
	targetStream->codingParameters.audio.sampleRate = sampleData.sampleRate;

	UniquePointer<Frame> audioFrame;

	UniquePointer<Packet> packet = EncodeAudio(*audioFrame, targetStream->codingParameters.codingFormat->GetId(), *targetStream);

	muxer->WriteHeader();

	//write an audio packet
	muxer->WritePacket(*packet);
	muxer->Finalize();
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument setPathArg(u8"sourceSet-path", u8"Path to the sourceSet");
	parser.AddPositionalArgument(setPathArg);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	FileSystem::Path setPath = setPathArg.Value(result);

	Set set(setPath);
	for(const auto& bankEntry : set.sampleBanks.Entries())
	{
		stdOut << u8"Exporting bank: " << bankEntry.bankNumber.ToString() << endl;

		FileSystem::Path bankPath = bankEntry.bankNumber.ToString();
		FileSystem::File bankDir(bankPath);
		bankDir.CreateDirectory();

		uint32 i = 0;
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const String& sampleName = objectEntry.name;
			const AbstractSample& sample = *objectEntry.object;

			if(IS_INSTANCE_OF(&sample, EncryptedSample))
			{
				stdOut << u8"Skipping encrypted sample: " << sampleName << endl;
			}
			else
			{
				const SampleObject& sampleObject = dynamic_cast<const SampleObject&>(sample);

				stdOut << u8"Exporting sample: " << sampleName << endl;

				const MultiSamples::SampleEntry* sampleEntry = nullptr;
				for(const auto& multiSampleSampleEntry : set.MultiSamples().data.sampleEntries)
				{
					if(multiSampleSampleEntry.id == sampleObject.data.id)
					{
						sampleEntry = &multiSampleSampleEntry;
						break;
					}
				}

				ExportWave(bankPath / (String::Number(i) + u8"_" + sampleName.Trim()), sampleObject.data, sampleEntry);
			}
			i++;
		}
	}

	return EXIT_SUCCESS;
}
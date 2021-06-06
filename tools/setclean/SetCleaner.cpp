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
//Class header
#include "SetCleaner.hpp"

//Public methods
void SetCleaner::RemoveUnusedMultiSamples()
{
	this->ProcessSounds();

	uint32 count = 0;
	stdOut << u8"Position\tName" << endl;
	for(uint32 i = 0; i < this->set.MultiSamples().data.multiSampleEntries.GetNumberOfElements(); i++)
	{
		const auto& msEntry = this->set.MultiSamples().data.multiSampleEntries[i];
		if(this->markedMultiSamples.Contains(msEntry.id))
			continue;

		count++;
		stdOut << i << u8"\t" << msEntry.name << endl;
	}

	TextReader textReader(stdIn, TextCodecType::UTF8);
	stdOut << endl << u8"Are you sure that you want to remove the above " << count << u8" multisamples? Type 'y' if you want to proceed, or anything else to cancel: ";
	if(textReader.ReadLine() == u8"y")
	{
		//TODO: remove keyzones
		//TODO: renumber keyzone indexes in sounds
		//TODO: renumber multisample numbers in sounds
	}
	else
	{
		stdOut << u8"Aborted. Set wasn't changed." << endl;
	}
}

void SetCleaner::RemoveUnusedSamples()
{
	uint32 oldSampleRamSize = this->set.ComputeUsedSampleRAMSize();

	this->ProcessMultiSamples();
	this->ProcessSounds();

	uint32 samplesCount = 0, drumSamplesCount = 0;
	stdOut << u8"Position\tName" << endl
		<< u8"Samples" << endl;
	for(uint32 i = 0; i < this->set.MultiSamples().data.sampleEntries.GetNumberOfElements(); i++)
	{
		const auto& entry = this->set.MultiSamples().data.sampleEntries[i];
		if(this->markedSamples.Contains(entry.id))
			continue;

		samplesCount++;
		stdOut << i << u8"\t" << entry.name << endl;
	}
	stdOut << u8"Drum samples" << endl;
	for(uint32 i = 0; i < this->set.MultiSamples().data.drumSampleEntries.GetNumberOfElements(); i++)
	{
		const auto& entry = this->set.MultiSamples().data.drumSampleEntries[i];
		if(this->markedDrumSamples.Contains(entry.id))
			continue;

		drumSamplesCount++;
		stdOut << i << u8"\t" << entry.name << endl;
	}

	TextReader textReader(stdIn, TextCodecType::UTF8);
	stdOut << endl << u8"Are you sure that you want to remove the above " << samplesCount << u8" samples and the " << drumSamplesCount << u8" drum samples? Type 'y' if you want to proceed, or anything else to cancel: ";
	if(textReader.ReadLine() == u8"y")
	{
		uint32 newSampleRamSize = this->set.ComputeUsedSampleRAMSize();
		stdOut << u8"Saved." << endl
			<< u8"Old sample RAM usage:"
			<< u8"Occupied: " << String::FormatBinaryPrefixed(oldSampleRamSize) << endl
			<< u8"Free: " << String::FormatBinaryPrefixed(this->set.model.GetSampleRAMSize() * MiB - oldSampleRamSize) << endl
			<< u8"New sample RAM usage:"
			<< u8"Occupied: " << String::FormatBinaryPrefixed(newSampleRamSize) << endl
			<< u8"Free: " << String::FormatBinaryPrefixed(this->set.model.GetSampleRAMSize() * MiB - newSampleRamSize) << endl;
	}
	else
	{
		stdOut << u8"Aborted. Set wasn't changed." << endl;
	}
}

void SetCleaner::RemoveUnusedSounds()
{
	this->ProcessPads();
	this->ProcessPerformances();
	this->ProcessStyles();

	uint32 count = 0;
	stdOut << u8"Bank\tPosition\tName" << endl;
	for(const auto& bankEntry : this->set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			if(this->markedSounds.Contains(Set::CreateRAMSoundProgramChangeSequence(bankEntry.bankNumber, objectEntry.pos)))
				continue;

			count++;
			stdOut << bankEntry.bankNumber.ToString() << u8"\t" << BankPositionToString(objectEntry.pos) << u8"\t" << objectEntry.name << endl;
		}
	}

	TextReader textReader(stdIn, TextCodecType::UTF8);
	stdOut << endl << u8"Are you sure that you want to remove the above " << count << u8" sounds? Type 'y' if you want to proceed, or anything else to cancel: ";
	if(textReader.ReadLine() == u8"y")
	{
	}
	else
	{
		stdOut << u8"Aborted. Set wasn't changed." << endl;
	}
}

//Private methods
void SetCleaner::ProcessMultiSamples()
{
	const auto& data = this->set.MultiSamples().data;
	for(const auto& msEntry : data.multiSampleEntries)
	{
		for(uint8 relativeIndex : msEntry.keyZoneIndex)
		{
			if(relativeIndex == Unsigned<uint8>::Max())
				continue;
			uint16 index = msEntry.keyZoneBaseIndex + relativeIndex;
			int16 sampleNumber = data.keyboardZones[index].sampleNumber;
			if(sampleNumber == -1)
				continue;

			this->markedSamples.Insert(data.sampleEntries[sampleNumber].id);
		}
	}
}

void SetCleaner::ProcessPads()
{
	for(const auto& bankEntry : this->set.padBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			this->ProcessStyle(objectEntry.object->data);
		}
	}
}

void SetCleaner::ProcessPerformances()
{
	for(const auto& bankEntry : this->set.performanceBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const auto& perf = *objectEntry.object;
			switch(perf.Version())
			{
				case 0:
				{
					for(const auto& track : perf.V0Data().keyboardSettings.trackSettings)
						this->markedSounds.Insert(track.soundProgramChangeSeq);
					for(const auto& track : perf.V0Data().unknownPart8)
						this->markedSounds.Insert(track.soundProgramChangeSeq);
				}
				break;
				case 1:
				{
					for(const auto& track : perf.V1Data().keyboardSettings.trackSettings)
						this->markedSounds.Insert(track.soundProgramChangeSeq);
					for(const auto& track : perf.V1Data().accompanimentSettings.trackSettings)
						this->markedSounds.Insert(track.soundProgramChangeSeq);
				}
					break;
				case 2:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
	}
}

void SetCleaner::ProcessSounds()
{
	for(const auto& bankEntry : this->set.soundBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			const auto& data = objectEntry.object->data;

			for(const auto& osc : data.oscillators)
			{
				this->markedMultiSamples.Insert(osc.high.multiSampleId);
				this->markedMultiSamples.Insert(osc.low.multiSampleId);
			}

			if(data.drumKitData.HasValue())
			{
				const auto& dkData = data.drumKitData.Value();
				for(const auto& layer : dkData.layers)
				{
					//TODO:
				}
			}
		}
	}
}

void SetCleaner::ProcessSTS(const SingleTouchSettings& sts)
{
	switch(sts.Version())
	{
		case 0:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		case 1:
		{
			for(const auto& kbdSettings : sts.V1Data().keyboardSettings)
			{
				for(const auto& track : kbdSettings.trackSettings)
					this->markedSounds.Insert(track.soundProgramChangeSeq);
			}
			for(const auto& track : sts.V1Data().accompanimentSettings.trackSettings)
				this->markedSounds.Insert(track.soundProgramChangeSeq);
		}
		break;
		case 2:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

void SetCleaner::ProcessStyle(const Style::StyleData &styleData)
{
	for(const auto& var : styleData.variation)
	{
		for(const auto& track : var.styleTrackData)
			this->markedSounds.Insert(track.soundProgramChangeSeq);
	}

	for(const auto& var : styleData.styleElements)
	{
		for(const auto& track : var.styleTrackData)
			this->markedSounds.Insert(track.soundProgramChangeSeq);
	}
}

void SetCleaner::ProcessStyles()
{
	for(const auto& bankEntry : this->set.styleBanks.Entries())
	{
		for(const auto& objectEntry : bankEntry.bank.Objects())
		{
			this->ProcessSTS(objectEntry.object->STS());
			this->ProcessStyle(objectEntry.object->Style().data);
		}
	}
}
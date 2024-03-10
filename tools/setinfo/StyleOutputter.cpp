/*
 * Copyright (c) 2020-2024 Amir Czwink (amir130@hotmail.de)
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
#include "StyleOutputter.hpp"
//Local
#include "PerformanceOutputter.hpp"

//Public methods
void StyleOutputter::Output(const FullStyle &fullStyle)
{
	this->Output(fullStyle.Style());

	PerformanceOutputter performanceOutputter(this->formattedOutputter);
	performanceOutputter.Output(fullStyle.STS());
}

void StyleOutputter::Output(const StyleObject &style)
{
	Section section(u8"Style Data", this->formattedOutputter);
	this->Output(style.data);
}

//Private methods
void StyleOutputter::Output(const ChordTable &chordTable)
{
	Section chunkSection(u8"Chord table", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"majorCVIndex", chordTable.majorCVIndex);
	this->formattedOutputter.OutputProperty(u8"sixCVIndex", chordTable.sixCVIndex);
	this->formattedOutputter.OutputProperty(u8"M7CVIndex", chordTable.M7CVIndex);
	this->formattedOutputter.OutputProperty(u8"M7b5CVIndex", chordTable.M7b5CVIndex);
	this->formattedOutputter.OutputProperty(u8"susCVIndex", chordTable.susCVIndex);
	this->formattedOutputter.OutputProperty(u8"sus2CVIndex", chordTable.sus2CVIndex);
	this->formattedOutputter.OutputProperty(u8"M7susCVIndex", chordTable.M7susCVIndex);
	this->formattedOutputter.OutputProperty(u8"mCVIndex", chordTable.mCVIndex);
	this->formattedOutputter.OutputProperty(u8"m6CVIndex", chordTable.m6CVIndex);
	this->formattedOutputter.OutputProperty(u8"m7CVIndex", chordTable.m7CVIndex);
	this->formattedOutputter.OutputProperty(u8"m7b5CVIndex", chordTable.m7b5CVIndex);
	this->formattedOutputter.OutputProperty(u8"mM7CVIndex", chordTable.mM7CVIndex);
	this->formattedOutputter.OutputProperty(u8"sevenCVIndex", chordTable.sevenCVIndex);
	this->formattedOutputter.OutputProperty(u8"seven_b5CVIndex", chordTable.seven_b5CVIndex);
	this->formattedOutputter.OutputProperty(u8"seven_susCVIndex", chordTable.seven_susCVIndex);
	this->formattedOutputter.OutputProperty(u8"dimCVIndex", chordTable.dimCVIndex);
	this->formattedOutputter.OutputProperty(u8"dimM7CVIndex", chordTable.dimM7CVIndex);
	this->formattedOutputter.OutputProperty(u8"sharp5CVIndex", chordTable.sharp5CVIndex);
	this->formattedOutputter.OutputProperty(u8"seven_sharp5CVIndex", chordTable.seven_sharp5CVIndex);
	this->formattedOutputter.OutputProperty(u8"M7sharp5CVIndex", chordTable.M7sharp5CVIndex);
	this->formattedOutputter.OutputProperty(u8"onePlusFiveCVIndex", chordTable.onePlusFiveCVIndex);
	this->formattedOutputter.OutputProperty(u8"onePlusEightCVIndex", chordTable.onePlusEightCVIndex);
	this->formattedOutputter.OutputProperty(u8"b5CVIndex", chordTable.b5CVIndex);
	this->formattedOutputter.OutputProperty(u8"dim7CVIndex", chordTable.dim7CVIndex);
}

void StyleOutputter::Output(uint32 index, const ChordVariationData &data)
{
	Section cvSection(u8"Chord variation " + String::Number(index+1), this->formattedOutputter);

	this->Output(data.masterMidiTrack);

	{
		Section trackMappingSection(u8"Track mapping", this->formattedOutputter);
		for(uint32 i = 0; i < data.trackMapping.GetNumberOfElements(); i++)
		{
			this->formattedOutputter.OutputProperty(u8"track type " + String::Number(i), this->ToString(data.trackMapping[i].type));
			this->formattedOutputter.OutputProperty(u8"track number " + String::Number(i), AccompanimentTrackNumberToAbbreviatedString(data.trackMapping[i].trackNumber));
		}
	}
}

void StyleOutputter::Output(const StyleData &styleData)
{
	this->Output(styleData.styleInfoData);

	{
		Section chunkSection(u8"MIDI track mapping", this->formattedOutputter);
		for(uint32 i = 0; i < styleData.oneBasedMIDITrackMappingIndices.GetNumberOfElements(); i++)
			this->formattedOutputter.OutputProperty(String::Number(i), styleData.oneBasedMIDITrackMappingIndices[i]);
	}

	{
		Section chunkSection(u8"MIDI tracks", this->formattedOutputter);
		for(uint32 i = 0; i < styleData.midiTracks.GetNumberOfElements(); i++)
		{
			const MIDI_Track& midiTrack = styleData.midiTracks[i];

			Section trackSection(u8"MIDI track #" + String::Number(i), this->formattedOutputter);
			this->Output(midiTrack);
		}
	}

	StyleView styleView(styleData);

	Section styleElements(u8"Style elements", this->formattedOutputter);
	for(uint8 i = 0; i < 4; i++)
	{
		if(!styleView.IsVariationDataAvailable(i))
			continue;

		Section variationSection(u8"Variation " + String::Number(i+1), this->formattedOutputter);
		this->Output(styleData.variation[i]);
	}

	for(uint8 i = 0; i < 11; i++)
	{
		auto styleElementNumber = (StyleElementNumber)i;
		if(!styleView.IsStyleElementDataAvailable(styleElementNumber))
			continue;

		Section styleElementSection(this->ToString(styleElementNumber), this->formattedOutputter);
		this->Output(styleData.styleElements[i]);
	}
}

void StyleOutputter::Output(const StyleElementInfoData& styleElementInfoData)
{
	Section chunkSection(u8"StyleElementInfoData", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"chordVariationsWithData", styleElementInfoData.chordVariationsWithData);
	this->formattedOutputter.OutputProperty(u8"unknown2", styleElementInfoData.unknown2);
	this->Output(styleElementInfoData.chordTable);
	this->formattedOutputter.OutputProperty(u8"cueMode", (uint8)styleElementInfoData.cueMode);
	this->formattedOutputter.OutputUnknownProperties(styleElementInfoData.unknown4, sizeof(styleElementInfoData.unknown4));
	this->formattedOutputter.OutputUnknownProperties(styleElementInfoData.unknown5, sizeof(styleElementInfoData.unknown5));
	this->formattedOutputter.OutputProperty(u8"unknown6", styleElementInfoData.unknown6);
}

void StyleOutputter::Output(const StyleInfoData& styleInfoData)
{
	auto& data = styleInfoData;

	Section chunkSection(u8"StyleInfoData", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"name", data.name);
	this->formattedOutputter.OutputProperty(u8"unknown111", data.unknown111);
	this->formattedOutputter.OutputProperty(u8"unknown3", data.unknown3);
	this->formattedOutputter.OutputProperty(u8"enabledStyleElements", data.enabledStyleElements);
	this->formattedOutputter.OutputProperty(u8"unknown6", data.unknown6);
	this->formattedOutputter.OutputProperty(u8"unknown7", data.unknown7);
	this->formattedOutputter.OutputProperty(u8"unknown8", data.unknown8);
	this->formattedOutputter.OutputProperty(u8"unknown9", data.unknown9);
	this->formattedOutputter.OutputProperty(u8"unknown10", data.unknown10);
	this->formattedOutputter.OutputProperty(u8"unknown11", data.unknown11);
	this->formattedOutputter.OutputProperty(u8"unknown12", data.unknown12);
	this->formattedOutputter.OutputProperty(u8"unknown13", data.unknown13);
	this->formattedOutputter.OutputProperty(u8"unknown14", data.unknown14);
	this->formattedOutputter.OutputProperty(u8"unknown15", data.unknown15);
	this->formattedOutputter.OutputProperty(u8"unknown16", data.unknown16);
	this->formattedOutputter.OutputProperty(u8"styleElementsWithData", data.styleElementsWithData);
}

void StyleOutputter::Output(const MasterMIDI_Track &midiTrack)
{
	Section section(u8"Master MIDI track", this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack.unknown1);
	this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack.unknown2);
	this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack.unknown3);

	for(uint32 i = 0; i < midiTrack.events.GetNumberOfElements(); i++)
	{
		this->Output(i, midiTrack.events[i]);
	}
}

void StyleOutputter::Output(const MIDI_Track& midiTrack)
{
	this->formattedOutputter.OutputProperty(u8"chunkId", String::HexNumber(midiTrack.chunkType));
	switch(midiTrack.chunkType)
	{
		case MIDI_Track::CHUNK_0x2000008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x2000008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x2000008_data.unknown2);
			break;
		case MIDI_Track::CHUNK_0x3000008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x3000008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x3000008_data.unknown2);
			this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack._0x3000008_data.unknown3);
			this->formattedOutputter.OutputProperty(u8"unknown4", midiTrack._0x3000008_data.unknown4);
			this->formattedOutputter.OutputProperty(u8"unknown5", midiTrack._0x3000008_data.unknown5);
			break;
		case MIDI_Track::CHUNK_0x4000008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x4000008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x4000008_data.unknown2);
			this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack._0x4000008_data.unknown3);
			this->formattedOutputter.OutputProperty(u8"unknown4", midiTrack._0x4000008_data.unknown4);
			break;
		case MIDI_Track::CHUNK_0x5010008:
			this->formattedOutputter.OutputProperty(u8"unknown1", midiTrack._0x5010008_data.unknown1);
			this->formattedOutputter.OutputProperty(u8"unknown2", midiTrack._0x5010008_data.unknown2);
			this->formattedOutputter.OutputProperty(u8"unknown3", midiTrack._0x5010008_data.unknown3);
			this->formattedOutputter.OutputProperty(u8"unknown4", midiTrack._0x5010008_data.unknown4);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}

	for(uint32 i = 0; i < midiTrack.events.GetNumberOfElements(); i++)
	{
		this->Output(i, midiTrack.events[i]);
	}
}

void StyleOutputter::Output(uint32 index, const KORG_MIDI_Event &event)
{
	Section eventSection(u8"MIDI event #" + String::Number(index), this->formattedOutputter);

	switch(event.type)
	{
		case KORG_MIDI_EventType::NoteOff:
			this->formattedOutputter.OutputProperty(u8"type", u8"note off");
			this->formattedOutputter.OutputProperty(u8"pitch", Pitch(event.value1).ToString());
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			this->formattedOutputter.OutputProperty(u8"unknownAdditional", event.unknownAdditional.HasValue() ? String::Number(event.unknownAdditional.Value()) : u8"N/A");
			break;
		case KORG_MIDI_EventType::NoteOn:
			this->formattedOutputter.OutputProperty(u8"type", u8"note on");
			this->formattedOutputter.OutputProperty(u8"pitch", Pitch(event.value1).ToString());
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			this->formattedOutputter.OutputProperty(u8"unknownAdditional", event.unknownAdditional.HasValue() ? String::Number(event.unknownAdditional.Value()) : u8"N/A");
			break;
		case KORG_MIDI_EventType::ControlChange:
			this->formattedOutputter.OutputProperty(u8"type", u8"control change");
			this->formattedOutputter.OutputProperty(u8"number", event.value1);
			this->formattedOutputter.OutputProperty(u8"value", event.value2);
			this->formattedOutputter.OutputProperty(u8"unknownAdditional", event.unknownAdditional.HasValue() ? String::Number(event.unknownAdditional.Value()) : u8"N/A");
			break;
		case KORG_MIDI_EventType::Aftertouch:
			this->formattedOutputter.OutputProperty(u8"type", u8"aftertouch");
			this->formattedOutputter.OutputProperty(u8"velocity", event.value1);
			break;
		case KORG_MIDI_EventType::Bend:
			this->formattedOutputter.OutputProperty(u8"type", u8"bend");
			this->formattedOutputter.OutputProperty(u8"velocity", (int16)event.value1);
			this->formattedOutputter.OutputProperty(u8"unknownAdditional", event.unknownAdditional.HasValue() ? String::Number(event.unknownAdditional.Value()) : u8"N/A");
			break;
		case KORG_MIDI_EventType::RXnoiseOff:
			this->formattedOutputter.OutputProperty(u8"type", u8"rxnoise off");
			this->formattedOutputter.OutputProperty(u8"pitch", Pitch(event.value1).ToString());
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			break;
		case KORG_MIDI_EventType::RXnoiseOn:
			this->formattedOutputter.OutputProperty(u8"type", u8"rxnoise on");
			this->formattedOutputter.OutputProperty(u8"pitch", Pitch(event.value1).ToString());
			this->formattedOutputter.OutputProperty(u8"velocity", event.value2);
			break;
		case KORG_MIDI_EventType::MetaEvent:
			this->formattedOutputter.OutputProperty(u8"type", u8"meta");
			this->formattedOutputter.OutputProperty(u8"meta event type", (uint8)event.metaEvent.type);
			this->formattedOutputter.OutputProperty(u8"meta dataLength", (uint8)event.metaEvent.dataLength);
			{
				Section section(u8"data", this->formattedOutputter);
				for(uint8 i = 0; i < event.metaEvent.dataLength; i++)
					this->formattedOutputter.OutputProperty(u8"byte", event.metaEvent.data[i]);
			}
			break;
		case KORG_MIDI_EventType::DeltaTime:
			this->formattedOutputter.OutputProperty(u8"type", u8"delta time");
			this->formattedOutputter.OutputProperty(u8"velocity", event.value1);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

void StyleOutputter::Output(const VariationStyleElementData& data)
{
	this->OutputGeneralStyleElementData(data);

	for(uint32 i = 0; i < 6; i++)
	{
		if(data.styleElementInfoData.IsChordVariationDataAvailable(i))
			this->Output(i, data.cv[i]);
	}
}

void StyleOutputter::Output(const StyleElementData &data)
{
	this->OutputGeneralStyleElementData(data);

	for(uint32 i = 0; i < 2; i++)
	{
		if(data.styleElementInfoData.IsChordVariationDataAvailable(i))
			this->Output(i, data.cv[i]);
	}
}

void StyleOutputter::OutputGeneralStyleElementData(const GeneralStyleElementData &data)
{
	this->Output(data.styleElementInfoData);

	for(uint32 i = 0; i < 8; i++)
	{
		Section trackSection(AccompanimentTrackNumberToAbbreviatedString(static_cast<AccompanimentTrackNumber>(i)), this->formattedOutputter);
		this->formattedOutputter.OutputProperty(u8"expression", data.styleTrackData[i].expression);
		this->formattedOutputter.OutputProperty(u8"soundProgramChangeSeq", data.styleTrackData[i].soundProgramChangeSeq.ToString());
		this->formattedOutputter.OutputProperty(u8"keyboardRangeBottom", data.styleTrackData[i].keyboardRangeBottom);
		this->formattedOutputter.OutputProperty(u8"keyboardRangeTop", data.styleTrackData[i].keyboardRangeTop);
		this->formattedOutputter.OutputProperty(u8"NTT_type", (uint8)data.styleTrackData[i].ntt.Type());
		this->formattedOutputter.OutputProperty(u8"NTT_table", (data.styleTrackData[i].ntt.Type() == NTT_Type::Parallel) ? (uint8)data.styleTrackData[i].ntt.TableParallel() : (uint8)data.styleTrackData[i].ntt.TableFixed());
	}

	for(uint32 i = 0; i < 8; i++)
	{
		Section section(u8"unknown1_" + String::Number(i), this->formattedOutputter);

		this->formattedOutputter.OutputUnknownProperties(data.styleTrackData[i].unknown1, sizeof(data.styleTrackData[i].unknown1));
	}

	for(uint32 i = 0; i < 8; i++)
	{
		Section section(u8"unknown3_" + String::Number(i), this->formattedOutputter);

		this->formattedOutputter.OutputProperty(u8"unknown3", data.styleTrackData[i].unknown3);
	}
}

String StyleOutputter::ToString(ChordVariationTrackType trackType) const
{
	switch(trackType)
	{
		case ChordVariationTrackType::DrumOrPerc:
			return u8"DRUM/PERC";
		case ChordVariationTrackType::Bass:
			return u8"BASS";
		case ChordVariationTrackType::Accompany:
			return u8"ACC";
	}

	RAISE(ErrorHandling::IllegalCodePathError);
}

String StyleOutputter::ToString(StyleElementNumber styleElementNumber) const
{
	switch (styleElementNumber)
	{
		case StyleElementNumber::Break:
			return u8"Break";
		case StyleElementNumber::Ending1:
			return u8"Ending 1";
		case StyleElementNumber::Ending2:
			return u8"Ending 2";
		case StyleElementNumber::Ending3:
			return u8"Ending 3";
		case StyleElementNumber::Intro1:
			return u8"Intro 1";
		case StyleElementNumber::Intro2:
			return u8"Intro 2";
		case StyleElementNumber::Intro3:
			return u8"Intro 3";
		case StyleElementNumber::Fill1:
			return u8"Fill 1";
		case StyleElementNumber::Fill2:
			return u8"Fill 2";
		case StyleElementNumber::Fill3:
			return u8"Fill 3";
		case StyleElementNumber::Fill4:
			return u8"Fill 4";
	}

	RAISE(ErrorHandling::IllegalCodePathError);
}
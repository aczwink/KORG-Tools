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
#include "PerformanceWriterV1_0Writer.hpp"
//Local
#include "../../Reading/PerformanceDataFormat.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Performance;

//Public methods
void PerformanceWriterV1_0Writer::Write(const SingleTouchSettings &settings)
{
	NOT_IMPLEMENTED_ERROR; //TODO: refix me
	/*
	this->BeginChunk(0, 0, 0, 0);
	this->BeginChunk(1, 0, 0, 0);

	for(const auto& chunk : settings.unknownChunksAtBeginning)
		this->WriteUnknownChunk(chunk);
	this->Write0x2000000Chunk(settings.accompanimentSettings, settings.Max9Version());

	for(uint8 i = 0; i < 4; i++)
	{
		this->Write0x3000000Chunk(settings.keyboardSettings[i], i, settings.Max9Version());
	}

	if(!settings.unknownChunksAtEnd.IsEmpty())
	{
		this->BeginChunk(28, settings.Max9Version().minor, 0, 0);
		for(const auto& chunk : settings.unknownChunksAtEnd)
			this->WriteUnknownChunk(chunk);
		this->EndChunk();
	}

	this->EndChunk();
	this->EndChunk();*/
}

//Private methods
void PerformanceWriterV1_0Writer::Write0x2000000Chunk(const AccompanimentSettings& settings, const ChunkVersion& performanceVersion)
{
	NOT_IMPLEMENTED_ERROR; //TODO: refix me
	/*this->BeginChunk(2, 0, 0, 0);
	this->BeginChunk(5, performanceVersion.major, 0, 0);

	for(const auto& chunk : settings.unknownChunksBefore9)
		this->WriteUnknownChunk(chunk);

	for(uint32 i = 0; i < settings.trackProperties.Size(); i++)
	{
		this->WriteTrackProperties(i, settings.trackProperties[i], performanceVersion);
	}

	for(const auto& chunk : settings.unknownChunksAfter9)
		this->WriteUnknownChunk(chunk);

	this->EndChunk();
	this->EndChunk();*/
}

void PerformanceWriterV1_0Writer::Write0x3000000Chunk(const KeyboardSettings &keyboardSettings, uint8 number, const ChunkVersion& performanceVersion)
{
	NOT_IMPLEMENTED_ERROR; //TODO: refix me
	/*TextWriter textWriter(this->outputStream, TextCodecType::ASCII);

	this->BeginChunk(3, 0, 0, 0);
	this->BeginChunk(10, performanceVersion.major, 0, 0);

	this->BeginChunk(15, 0, 0, ChunkHeaderFlags::Leaf);
	this->dataWriter.WriteUInt32(number);
	textWriter.WriteFixedLengthString(keyboardSettings.name, 37);
	this->EndChunk();

	for(const auto& chunk : keyboardSettings.unknownChunksBeforeTracks)
		this->WriteUnknownChunk(chunk);

	for(uint32 i = 0; i < keyboardSettings.trackProperties.Size(); i++)
	{
		this->WriteTrackProperties(i, keyboardSettings.trackProperties[i], performanceVersion);
	}

	for(const auto& chunk : keyboardSettings.unknownChunksAfterTracks)
		this->WriteUnknownChunk(chunk);

	this->EndChunk();
	this->EndChunk();*/
}

void PerformanceWriterV1_0Writer::WriteTrackProperties(uint32 trackNumber, const TrackProperties &trackProperties, const libKORG::ChunkVersion& performanceVersion)
{
	NOT_IMPLEMENTED_ERROR; //TODO: refix me
	/*
	this->BeginChunk(9, performanceVersion.major, performanceVersion.minor, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteUInt32(trackNumber);

	trackProperties.unknown1.CreateInputStream()->FlushTo(this->outputStream);
	this->dataWriter.WriteByte(trackProperties.soundProgramChangeSeq.BankSelectMSB());
	this->dataWriter.WriteByte(trackProperties.soundProgramChangeSeq.BankSelectLSB());
	this->dataWriter.WriteByte(static_cast<byte>(trackProperties.soundProgramChangeSeq.SoundSetType()));
	this->dataWriter.WriteByte(trackProperties.soundProgramChangeSeq.ProgramChange());
	this->dataWriter.WriteByte(trackProperties.volume);
	this->dataWriter.WriteByte(trackProperties.pan + c_knob_offset);
	this->dataWriter.WriteByte(trackProperties.detune + c_knob_offset);
	this->dataWriter.WriteByte(trackProperties.octaveTranspose * 12);
	trackProperties.unknown2.CreateInputStream()->FlushTo(this->outputStream);
	this->dataWriter.WriteByte(trackProperties.fxMaster1);
	this->dataWriter.WriteByte(trackProperties.fxMaster2);
	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(trackProperties.dry);
	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(trackProperties.unknown12);
	this->dataWriter.WriteByte(trackProperties.unknown13);
	this->dataWriter.WriteByte(trackProperties.pbSensitivity);
	this->dataWriter.WriteByte(trackProperties.unknown182);
	this->dataWriter.WriteByte(trackProperties.unknown181);
	this->dataWriter.WriteByte(trackProperties.lowGainTimes2);
	this->dataWriter.WriteByte(trackProperties.midGainTimes2);
	this->dataWriter.WriteByte(trackProperties.highGainTimes2);
	trackProperties.unknown3.CreateInputStream()->FlushTo(this->outputStream);

	this->EndChunk();
	 */
}
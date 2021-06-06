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
using namespace libKORG::Performance::V1;
using namespace StdXX;

//Public methods
void PerformanceWriterV1_0Writer::Write(const PerformanceObject &performanceObject)
{
	const auto& data = performanceObject.V1Data();

	this->WriteGeneralStart(data);
	this->Write0x03000000Chunk(data.keyboardSettings, 4);
	this->WriteGeneralEnd(data);
}

void PerformanceWriterV1_0Writer::Write(const SingleTouchSettings &settings)
{
	const auto& data = settings.V1Data();

	this->WriteGeneralStart(data);

	for(uint8 i = 0; i < 4; i++)
		this->Write0x03000000Chunk(data.keyboardSettings[i], i);

	this->WriteGeneralEnd(data);
}

//Private methods
void PerformanceWriterV1_0Writer::Write0x02000000Chunk(const AccompanimentSettings& settings)
{
	this->BeginChunk(2, 0, 0, 0);
	this->BeginChunk(5, 0, 0, 0);

	this->Write0x06000008Chunk(settings._0x06000008_data);
	for(int32 index = 0; index < settings.fx.Size(); index++)
		this->Write0x07000008Chunk(index, settings.fx[index]);

	for(int32 index = 0; index < settings.padSettings.Size(); index++)
		this->Write0x08000008Chunk(index, settings.padSettings[index]);

	for(int32 index = 0; index < settings.trackSettings.Size(); index++)
		this->Write0x09000008Chunk(index, settings.trackSettings[index]);

	this->EndChunk();
	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x03000000Chunk(const KeyboardSettings &keyboardSettings, uint8 number)
{
	this->BeginChunk(3, 0, 0, 0);
	this->BeginChunk(10, 0, 0, 0);

	this->BeginChunk(15, 0, 0, ChunkHeaderFlags::Leaf);
	this->dataWriter.WriteUInt32(number);
	TextWriter textWriter(this->outputStream, TextCodecType::ASCII);
	textWriter.WriteFixedLengthString(keyboardSettings.name, 33);
	this->dataWriter.WriteUInt32(keyboardSettings.unknown1);
	this->EndChunk();

	this->Write0x10000008Chunk(keyboardSettings._0x10000008_data);
	this->Write0x11000008Chunk(keyboardSettings._0x11000008_data);
	this->Write0x12000108Chunk(keyboardSettings._0x12000108_data);
	this->Write0x13000008Chunk(keyboardSettings.scaleSettings);

	for(const auto& fx : keyboardSettings.fx)
	for(int32 index = 0; index < keyboardSettings.fx.Size(); index++)
		this->Write0x07000008Chunk(index, keyboardSettings.fx[index]);

	this->Write0x14000008Chunk(keyboardSettings._0x14000008_data);

	for(int32 index = 0; index < keyboardSettings.padSettings.Size(); index++)
		this->Write0x08000008Chunk(index, keyboardSettings.padSettings[index]);

	for(int32 index = 0; index < keyboardSettings._0x15000108_data.Size(); index++)
		this->Write0x15000108Chunk(index, keyboardSettings._0x15000108_data[index]);

	for(int32 index = 0; index < keyboardSettings.trackSettings.Size(); index++)
		this->Write0x09000008Chunk(index, keyboardSettings.trackSettings[index]);

	this->Write0x18000008Chunk(keyboardSettings._0x18000008_data);

	this->EndChunk();
	this->EndChunk();
}

/*void PerformanceWriterV1_0Writer::WriteTrackProperties(uint32 trackNumber, const TrackSettings &trackSettings, const libKORG::ChunkVersion& performanceVersion)
{
	NOT_IMPLEMENTED_ERROR; //TODO: refix me

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
	trackProperties.memoryBankNumber.CreateInputStream()->FlushTo(this->outputStream);
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

}*/

void PerformanceWriterV1_0Writer::Write0x04000108Chunk(const _0x04000108_chunk &_0x04000108_data)
{
	this->BeginChunk(4, 0, 1, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteInt32(0xF);
	this->dataWriter.WriteByte(_0x04000108_data.memoryBankNumber);
	this->dataWriter.WriteByte(_0x04000108_data.unknown3);
	this->dataWriter.WriteByte(_0x04000108_data.unknown4);
	this->dataWriter.WriteByte(_0x04000108_data.metronomeTempo);
	this->dataWriter.WriteBytes(_0x04000108_data.unknown41, sizeof(_0x04000108_data.unknown41));
	this->dataWriter.WriteBytes(_0x04000108_data.unknown5, sizeof(_0x04000108_data.unknown5));
	this->dataWriter.WriteBytes(_0x04000108_data.unknown6, sizeof(_0x04000108_data.unknown6));
	this->dataWriter.WriteBytes(_0x04000108_data.unknown7, sizeof(_0x04000108_data.unknown7));
	this->dataWriter.WriteBytes(_0x04000108_data.unknown8, sizeof(_0x04000108_data.unknown8));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x06000008Chunk(const _0x06000008_chunk &chunk)
{
	this->BeginChunk(6, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteInt8(chunk.unknown1);
	this->dataWriter.WriteInt8(chunk.unknown2);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x07000008Chunk(int32 index, const FXData &fxData)
{
	this->BeginChunk(7, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteInt32(index);
	this->dataWriter.WriteInt8(fxData.unknown2);
	this->dataWriter.WriteInt8(fxData.unknown3);
	this->dataWriter.WriteByte(fxData.unknown4);
	this->dataWriter.WriteByte(fxData.unknown5);
	this->dataWriter.WriteInt8(fxData.unknown6);
	this->dataWriter.WriteByte(fxData.unknown7);
	this->dataWriter.WriteByte(fxData.unknown8);
	this->dataWriter.WriteByte(fxData.unknown9);
	this->dataWriter.WriteInt16(fxData.unknown10);
	this->dataWriter.WriteBytes(fxData.unknown11, sizeof(fxData.unknown11));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x08000008Chunk(int32 index, const PadSettings &padSettings)
{
	this->BeginChunk(8, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteInt32(index);

	this->dataWriter.WriteUInt32(padSettings.unknown2);
	this->dataWriter.WriteByte(padSettings.unknown3);
	this->dataWriter.WriteInt8(padSettings.unknown4);
	this->dataWriter.WriteBytes(padSettings.unknown5, sizeof(padSettings.unknown5));
	this->dataWriter.WriteBytes(padSettings.unknown6, sizeof(padSettings.unknown6));
	this->dataWriter.WriteByte(padSettings.unknown7);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x09000008Chunk(int32 index, const TrackSettings &trackSettings)
{
	this->BeginChunk(9, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteInt32(index);

	this->dataWriter.WriteUInt32(trackSettings.unknown1);
	this->dataWriter.WriteBytes(trackSettings.unknown2, sizeof(trackSettings.unknown2));
	this->dataWriter.WriteUInt32(trackSettings.soundProgramChangeSeq.ToUInt32());
	this->dataWriter.WriteByte(trackSettings.volume);
	this->dataWriter.WriteByte(trackSettings.pan + c_knob_offset);
	this->dataWriter.WriteByte(trackSettings.detune + c_knob_offset);
	this->dataWriter.WriteInt8(trackSettings.octaveTranspose * 12);
	this->dataWriter.WriteBytes(trackSettings.unknown3, sizeof(trackSettings.unknown3));
	this->dataWriter.WriteByte(trackSettings.fxMaster1);
	this->dataWriter.WriteByte(trackSettings.fxMaster2);
	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(trackSettings.dry);
	this->dataWriter.WriteUInt32(trackSettings.unknown12);
	this->dataWriter.WriteByte(trackSettings.pbSensitivity);
	this->dataWriter.WriteByte(trackSettings.unknown182);
	this->dataWriter.WriteInt8(trackSettings.unknown181);
	this->dataWriter.WriteInt8(trackSettings.lowGainTimes2);
	this->dataWriter.WriteInt8(trackSettings.midGainTimes2);
	this->dataWriter.WriteInt8(trackSettings.highGainTimes2);
	this->dataWriter.WriteBytes(trackSettings.unknown6, sizeof(trackSettings.unknown6));

	for(uint32 unknown : trackSettings.unknown4)
		this->dataWriter.WriteUInt32(unknown);

	for(uint8 i = 0; i < 8; i++)
		this->dataWriter.WriteBytes(trackSettings.unknown5[i].unknown, sizeof(trackSettings.unknown5[i]));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x10000008Chunk(const _0x10000008_chunk &chunk)
{
	this->BeginChunk(0x10, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(chunk.unknown1);
	this->dataWriter.WriteByte(chunk.unknown2);
	this->dataWriter.WriteByte(chunk.unknown3);
	this->dataWriter.WriteByte(chunk.unknown4);
	this->dataWriter.WriteInt32(chunk.unknown5);
	this->dataWriter.WriteByte(chunk.unknown6);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x11000008Chunk(const _0x11000008_chunk &chunk)
{
	this->BeginChunk(0x11, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(chunk.unknown1);
	this->dataWriter.WriteByte(chunk.unknown2);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x12000108Chunk(const _0x12000108_chunk &chunk)
{
	this->BeginChunk(0x12, 0, 1, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteBytes(chunk.unknown1, sizeof(chunk.unknown1));
	this->dataWriter.WriteBytes(chunk.unknown2, sizeof(chunk.unknown2));
	this->dataWriter.WriteBytes(chunk.unknown3, sizeof(chunk.unknown3));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x13000008Chunk(const Performance::V0::ScaleSettings &scaleSettings)
{
	this->BeginChunk(0x13, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(static_cast<byte>(scaleSettings.scaleType));
	this->dataWriter.WriteByte(static_cast<byte>(scaleSettings.key));
	this->dataWriter.WriteBytes(scaleSettings.pitchTune, sizeof(scaleSettings.pitchTune));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x14000008Chunk(const _0x14000008_chunk &chunk)
{
	this->BeginChunk(0x14, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteBytes(chunk.unknown, sizeof(chunk.unknown));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x15000108Chunk(int32 index, const _0x15000108_chunk &chunk)
{
	this->BeginChunk(0x15, 0, 1, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteInt32(index);
	this->dataWriter.WriteInt32(chunk.unknown);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x18000008Chunk(const _0x18000008_chunk &chunk)
{
	this->BeginChunk(0x18, 0, 0, ChunkHeaderFlags::Leaf);

	for(uint8 i = 0; i < 9; i++)
		this->dataWriter.WriteUInt16(chunk.unknown1[i]);

	this->dataWriter.WriteBytes(chunk.unknown2, sizeof(chunk.unknown2));

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x1A000008Chunk(const _0x1A000008_chunk &chunk)
{
	this->BeginChunk(0x1A, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(chunk.unknown1);
	this->dataWriter.WriteByte(chunk.unknown2);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::Write0x1B000008Chunk(const _0x1B000008_chunk &chunk)
{
	this->BeginChunk(0x1B, 0, 0, ChunkHeaderFlags::Leaf);

	this->dataWriter.WriteByte(chunk.unknown1);
	this->dataWriter.WriteByte(chunk.unknown2);
	this->dataWriter.WriteByte(chunk.unknown3);

	this->EndChunk();
}

void PerformanceWriterV1_0Writer::WriteGeneralEnd(const GeneralData &data)
{
	this->BeginChunk(0x1C, 0, 0, 0);
	this->Write0x1B000008Chunk(data._0x1B000008_data);
	this->Write0x1A000008Chunk(data._0x1A000008_data);
	this->EndChunk();

	this->EndChunk();
	this->EndChunk();
}

void PerformanceWriterV1_0Writer::WriteGeneralStart(const GeneralData &data)
{
	this->BeginChunk(0, 0, 0, 0);
	this->BeginChunk(1, 0, 0, 0);

	this->Write0x04000108Chunk(data._0x04000108_data);
	this->Write0x02000000Chunk(data.accompanimentSettings);
}
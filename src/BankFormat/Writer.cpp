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
//Class header
#include "Writer.hpp"
//Local
#include "libkorg/BankFormat/Style.hpp"
#include "../PerformanceFormat/Format1.0/PerformanceWriterV1_0Writer.hpp"
#include "../StyleFormat/Format0.0/StyleFormat0_0V5_0Writer.hpp"
#include "../PCMFormat/PCMFormatWriter.hpp"
#include "../PCMFormat/PCMWriterFactory.hpp"
#include "../SoundFormat/SoundWriterFactory.hpp"
#include "../MultiSamplesFormat/MultiSamplesWriterFactory.hpp"
#include "../PerformanceFormat/PerformanceWriterFactory.hpp"
#include "../StyleFormat/StyleWriterFactory.hpp"
#include <libkorg/BankFormat/EncryptedSample.hpp>
#include <libkorg/BankFormat/SampleObject.hpp>
//Namespaces
using namespace BankFormat;

//Public methods
template<typename T>
void Writer::Write(const ObjectBank<T> &bank)
{
	this->WriteHeader();

	this->BeginCrossReferencedChunk(ChunkType::ObjectTOC, 0, 0, ChunkHeaderFlags::Leaf);
	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteTOCEntries(objectEntry.name, objectEntry.pos, *objectEntry.object);
	}
	this->EndChunk();

	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteObjects(objectEntry.pos, *objectEntry.object);
	}

	this->WriteCrossReferenceTable();

	this->EndChunk();
}

template void Writer::Write(const ObjectBank<AbstractSample> &bank);
template void Writer::Write(const ObjectBank<FullStyle> &bank);
template void Writer::Write(const ObjectBank<SoundObject> &bank);

void Writer::Write(const MultiSamplesObject &multiSamplesObject)
{
	this->WriteHeader();

	ChunkVersion dataVersion = this->model.GetSupportedResourceVersions().maxMultiSamplesVersion;

	this->BeginCrossReferencedChunk(ChunkType::ObjectTOC, 0, 0, ChunkHeaderFlags::Leaf);
	this->WriteTOCEntry(u8"RAM", 0, ObjectType::MultiSample, dataVersion);
	this->EndChunk();

	this->BeginCrossReferencedChunk(ChunkType::MultiSampleData, dataVersion.major, dataVersion.minor, ChunkHeaderFlags::Leaf);

	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);
	UniquePointer<MultiSamplesFormatWriter> bankObjectFormatWriter = CreateMultiSamplesWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(multiSamplesObject.data);
	bufferedOutputStream.Flush();

	this->EndChunk();

	this->WriteCrossReferenceTable();

	this->EndChunk();
}

/*void Writer::Write(const ObjectBank<FullStyle> &bank)
{
	this->WriteHeader();

	this->BeginCrossReferencedChunk(ChunkType::ObjectTOC, 0, 0, ChunkHeaderFlags::Leaf);
	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteTOCEntry(objectEntry.name, objectEntry.pos, ObjectType::Style, {0, 0});
		this->WriteTOCEntry(objectEntry.name, objectEntry.pos, ObjectType::StylePerformances, objectEntry.object->STS().Version());
	}
	this->EndChunk();

	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteStyle(objectEntry.object->Style());
		this->WriteSTS(objectEntry.object->STS());
	}

	this->WriteCrossReferenceTable();

	this->EndChunk();
}*/

//Private methods
ChunkVersion Writer::DeterminePCMVersion(const AbstractSample& sample) const
{
	const EncryptedSample* encryptedSample = dynamic_cast<const EncryptedSample *>(&sample);
	if(encryptedSample)
		NOT_IMPLEMENTED_ERROR; //TODO: version for encrypted should be saved

	return this->model.GetSupportedResourceVersions().maxPCMVersion;
}

void Writer::WriteCrossReferenceTable()
{
	this->BeginChunk(ChunkType::CrossReferenceTable, 0, 0, ChunkHeaderFlags::Leaf);

	this->fourccWriter.WriteUInt32(FOURCC(u8"KBEG"));
	for(uint32 offset : this->crossReferenceObjects)
	{
		this->dataWriter.WriteUInt32(offset);
	}
	this->fourccWriter.WriteUInt32(FOURCC(u8"KEND"));
	this->dataWriter.WriteUInt32(this->crossReferenceObjects.GetNumberOfElements());

	this->EndChunk();
}

void Writer::WriteHeader()
{
	this->BeginChunk(ChunkType::Container, 0, 1, ChunkHeaderFlags::Unknown4);

	this->BeginChunk(ChunkType::KorgFile, 0, 0, ChunkHeaderFlags::Leaf);
	this->dataWriter.WriteUInt32(0x0B000000);
	this->dataWriter.WriteUInt16(0);
	this->dataWriter.WriteByte(0);
	this->fourccWriter.WriteUInt32(FOURCC(u8"KORF"));
	this->dataWriter.WriteByte(0);

	this->EndChunk();
}

void Writer::WriteObjects(uint8 pos, const AbstractSample& sampleObject)
{
	ChunkVersion dataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::PCM);

	this->BeginCrossReferencedChunk(ChunkType::PCMData, dataVersion.major, dataVersion.minor, ChunkHeaderFlags::Leaf);
	this->WritePCMData(sampleObject, dataVersion);
	this->EndChunk();
}

void Writer::WriteObjects(uint8 pos, const FullStyle &fullStyle)
{
	ChunkVersion styleDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Style);
	this->BeginCrossReferencedChunk(ChunkType::StyleObject, styleDataVersion.major, styleDataVersion.minor, ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
	this->WriteStyle(fullStyle.Style(), styleDataVersion);
	this->EndChunk();

	ChunkVersion stylePerformancesDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::StylePerformances);
	this->BeginCrossReferencedChunk(ChunkType::PerformancesData, stylePerformancesDataVersion.major, stylePerformancesDataVersion.minor, ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
	this->WriteSTS(fullStyle.STS(), stylePerformancesDataVersion);
	this->EndChunk();
}

void Writer::WriteObjects(uint8 pos, const SoundObject &soundObject)
{
	ChunkVersion dataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Sound);

	this->BeginCrossReferencedChunk(ChunkType::SoundData, dataVersion.major, dataVersion.minor, ChunkHeaderFlags::Leaf);
	this->WriteSoundData(soundObject, dataVersion);
	this->EndChunk();
}

void Writer::WritePCMData(const AbstractSample& abstractSample, const ChunkVersion& dataVersion)
{
	const auto* encryptedSample = dynamic_cast<const EncryptedSample *>(&abstractSample);
	if(encryptedSample)
		NOT_IMPLEMENTED_ERROR; //TODO: version for encrypted should be saved

	const auto& sampleObject = dynamic_cast<const SampleObject &>(abstractSample);

	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);
	UniquePointer<PCMFormatWriter> bankObjectFormatWriter = CreatePCMWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(sampleObject.data);
	bufferedOutputStream.Flush();
}

void Writer::WriteSoundData(const SoundObject &soundObject, const ChunkVersion& dataVersion)
{
	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);

	UniquePointer<SoundFormatWriter> bankObjectFormatWriter = CreateSoundWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(soundObject.data);

	bufferedOutputStream.Flush();
}

void Writer::WriteSTS(const SingleTouchSettings &singleTouchSettings, const ChunkVersion& dataVersion)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();

	UniquePointer<PerformanceFormatWriter> bankObjectFormatWriter = CreatePerformanceWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(singleTouchSettings);

	buffer.CreateInputStream()->FlushTo(this->outputStream);
}

void Writer::WriteStyle(const StyleObject &style, const ChunkVersion& dataVersion)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();

	UniquePointer<StyleFormatWriter> bankObjectFormatWriter = CreateStyleWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(style.data);

	buffer.CreateInputStream()->FlushTo(this->outputStream);
}

void Writer::WriteTOCEntries(const String &name, uint8 pos, const AbstractSample &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::PCM, this->DeterminePCMVersion(object));
}

void Writer::WriteTOCEntries(const String &name, uint8 pos, const FullStyle &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Style, this->model.GetSupportedResourceVersions().maxStyleVersion);
	this->WriteTOCEntry(name, pos, ObjectType::StylePerformances, this->model.GetSupportedResourceVersions().maxPerformanceVersion);
}

void Writer::WriteTOCEntries(const String &name, uint8 pos, const SoundObject& object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Sound, this->model.GetSupportedResourceVersions().maxSoundVersion);
}

void Writer::WriteTOCEntry(const String& name, uint8 pos, ObjectType objectType, const ChunkVersion& version)
{
	TextWriter textWriter(this->outputStream, TextCodecType::ASCII);

	textWriter.WriteFixedLengthString(name, HEADERENTRY_NAME_SIZE);
	dataWriter.WriteByte(static_cast<byte>(objectType));
	dataWriter.WriteByte(0);
	dataWriter.WriteByte(pos);
	dataWriter.WriteByte(version.major);
	dataWriter.WriteByte(version.minor);
	dataWriter.WriteByte(0);

	this->objectVersionMap[pos][objectType] = version;
}
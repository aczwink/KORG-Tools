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
#include "ObjectBankWriter.hpp"
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
void ObjectBankWriter::Write(const ObjectBank<T> &bank)
{
	this->WriteHeader();

	this->BeginWritingIndex();
	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteTOCEntries(objectEntry.name, objectEntry.pos, *objectEntry.object);
	}
	this->EndIndex();

	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteObjects(objectEntry.pos, *objectEntry.object);
	}

	this->Finalize();
}

template void ObjectBankWriter::Write(const ObjectBank<AbstractSample> &bank);
template void ObjectBankWriter::Write(const ObjectBank<FullStyle> &bank);
template void ObjectBankWriter::Write(const ObjectBank<PerformanceObject> &bank);
template void ObjectBankWriter::Write(const ObjectBank<SoundObject> &bank);

void ObjectBankWriter::Write(const MultiSamplesObject &multiSamplesObject)
{
	this->WriteHeader();

	ChunkVersion dataVersion = this->model.GetSupportedResourceVersions().maxMultiSamplesVersion;

	this->BeginWritingIndex();
	this->WriteTOCEntry(u8"RAM", 0, ObjectType::MultiSample, dataVersion);
	this->EndIndex();

	this->BeginWritingObjectData();

	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);
	UniquePointer<MultiSamplesFormatWriter> bankObjectFormatWriter = CreateMultiSamplesWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(multiSamplesObject.data);
	bufferedOutputStream.Flush();

	this->EndWritingObject();

	this->Finalize();
}

/*void ObjectBankWriter::Write(const ObjectBank<FullStyle> &bank)
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
ChunkVersion ObjectBankWriter::DeterminePCMVersion(const AbstractSample& sample) const
{
	const EncryptedSample* encryptedSample = dynamic_cast<const EncryptedSample *>(&sample);
	if(encryptedSample)
		NOT_IMPLEMENTED_ERROR; //TODO: version for encrypted should be saved

	return this->model.GetSupportedResourceVersions().maxPCMVersion;
}

void ObjectBankWriter::WriteObjects(uint8 pos, const AbstractSample& sampleObject)
{
	ChunkVersion dataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::PCM);

	this->BeginWritingObjectData();
	this->WritePCMData(sampleObject, dataVersion);
	this->EndWritingObject();
}

void ObjectBankWriter::WriteObjects(uint8 pos, const FullStyle &fullStyle)
{
	ChunkVersion styleDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Style);
	this->BeginWritingObjectData();
	this->WriteStyle(fullStyle.Style(), styleDataVersion);
	this->EndWritingObject();

	ChunkVersion stylePerformancesDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::StylePerformances);
	this->BeginWritingObjectData();
	this->WriteSTS(fullStyle.STS(), stylePerformancesDataVersion);
	this->EndWritingObject();
}

void ObjectBankWriter::WriteObjects(uint8 pos, const PerformanceObject &performanceObject)
{
	ChunkVersion performancesDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Performance);
	this->BeginWritingObjectData();
	this->WritePerformance(performanceObject, performancesDataVersion);
	this->EndWritingObject();
}

void ObjectBankWriter::WriteObjects(uint8 pos, const SoundObject &soundObject)
{
	ChunkVersion dataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Sound);

	this->BeginWritingObjectData();
	this->WriteSoundData(soundObject, dataVersion);
	this->EndWritingObject();
}

void ObjectBankWriter::WritePCMData(const AbstractSample& abstractSample, const ChunkVersion& dataVersion)
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

void ObjectBankWriter::WritePerformance(const PerformanceObject& performance, const ChunkVersion &dataVersion)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();

	UniquePointer<PerformanceFormatWriter> bankObjectFormatWriter = CreatePerformanceWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(performance);

	buffer.CreateInputStream()->FlushTo(this->outputStream);
}

void ObjectBankWriter::WriteSoundData(const SoundObject &soundObject, const ChunkVersion& dataVersion)
{
	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);

	UniquePointer<SoundFormatWriter> bankObjectFormatWriter = CreateSoundWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(soundObject.data);

	bufferedOutputStream.Flush();
}

void ObjectBankWriter::WriteSTS(const SingleTouchSettings &singleTouchSettings, const ChunkVersion& dataVersion)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();

	UniquePointer<PerformanceFormatWriter> bankObjectFormatWriter = CreatePerformanceWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(singleTouchSettings);

	buffer.CreateInputStream()->FlushTo(this->outputStream);
}

void ObjectBankWriter::WriteStyle(const StyleObject &style, const ChunkVersion& dataVersion)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();

	UniquePointer<StyleFormatWriter> bankObjectFormatWriter = CreateStyleWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(style.data);

	buffer.CreateInputStream()->FlushTo(this->outputStream);
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const AbstractSample &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::PCM, this->DeterminePCMVersion(object));
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const FullStyle &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Style, this->model.GetSupportedResourceVersions().maxStyleVersion);
	this->WriteTOCEntry(name, pos, ObjectType::StylePerformances, this->model.GetSupportedResourceVersions().maxPerformanceVersion);
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const PerformanceObject &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Performance, this->model.GetSupportedResourceVersions().maxPerformanceVersion);
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const SoundObject& object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Sound, this->model.GetSupportedResourceVersions().maxSoundVersion);
}

void ObjectBankWriter::WriteTOCEntry(const String& name, uint8 pos, ObjectType objectType, const ChunkVersion& version)
{
	HeaderEntry headerEntry;
	headerEntry.name = name;
	headerEntry.pos = pos;
	headerEntry.type = objectType;
	headerEntry.dataVersion = version;
	this->WriteIndexEntry(headerEntry);
	this->objectVersionMap[pos][objectType] = version;
}
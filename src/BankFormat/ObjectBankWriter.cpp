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
void ObjectBankWriter::Write(const MultiSamplesObject &multiSamplesObject)
{
	this->WriteHeader();

	ChunkVersion dataVersion = this->model.GetSupportedResourceVersions().maxMultiSamplesVersion;

	this->BeginWritingIndex();
	this->WriteTOCEntry(u8"RAM", 0, ObjectType::MultiSample, dataVersion, ObjectStreamFormat::Compressed);
	this->EndIndex();

	auto compressor = this->BeginWritingObjectData();
	DataWriter dataWriter(true, *compressor);
	UniquePointer<MultiSamplesFormatWriter> bankObjectFormatWriter = CreateMultiSamplesWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(multiSamplesObject.data);
	compressor->Finalize();

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
	this->WriteStyle(fullStyle.Style(), styleDataVersion);

	ChunkVersion stylePerformancesDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::StylePerformances);
	this->WriteSTS(fullStyle.STS(), stylePerformancesDataVersion);
}

void ObjectBankWriter::WriteObjects(uint8 pos, const PerformanceObject &performanceObject)
{
	ChunkVersion performancesDataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Performance);
	UniquePointer<Compressor> compressor = this->BeginWritingObjectData();

	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();
	UniquePointer<PerformanceFormatWriter> bankObjectFormatWriter = CreatePerformanceWriter(*outputStream, performancesDataVersion);
	bankObjectFormatWriter->Write(performanceObject);

	buffer.CreateInputStream()->FlushTo(*compressor);
	compressor->Finalize();

	this->EndWritingObject();
}

void ObjectBankWriter::WriteObjects(uint8 pos, const SoundObject &soundObject)
{
	ChunkVersion dataVersion = this->objectVersionMap.Get(pos).Get(ObjectType::Sound);
	UniquePointer<Compressor> compressor = this->BeginWritingObjectData();

	DataWriter dataWriter(true, *compressor);
	UniquePointer<SoundFormatWriter> bankObjectFormatWriter = CreateSoundWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(soundObject.data);

	compressor->Finalize();

	this->EndWritingObject();
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const AbstractSample &object)
{
	const EncryptedSample* encryptedSample = dynamic_cast<const EncryptedSample *>(&object);

	HeaderEntry headerEntry;
	headerEntry.name = name;
	headerEntry.pos = pos;
	headerEntry.type = ObjectType::PCM;
	headerEntry.dataVersion = encryptedSample ? encryptedSample->DataVersion() : this->model.GetSupportedResourceVersions().maxPCMVersion;

	ObjectStreamFormat streamFormat = ObjectStreamFormat::Uncompressed; //PCM data does not compress well
	if(encryptedSample)
	{
		headerEntry.encryptionInformation = encryptedSample->EncryptionInfo();
		headerEntry.id = encryptedSample->GetId();
		streamFormat = encryptedSample->IsOC31Compressed() ? ObjectStreamFormat::EncryptedAndCompressed : ObjectStreamFormat::Encrypted;
	}

	this->WriteIndexEntry(headerEntry, streamFormat);
	this->objectVersionMap[pos][headerEntry.type] = headerEntry.dataVersion;
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const FullStyle &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Style, this->model.GetSupportedResourceVersions().maxStyleVersion, ObjectStreamFormat::Compressed);
	this->WriteTOCEntry(name, pos, ObjectType::StylePerformances, this->DeterminePerformanceVersion(object.STS().Version()), ObjectStreamFormat::Compressed);
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const PerformanceObject &object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Performance, this->DeterminePerformanceVersion(object.Version()), ObjectStreamFormat::Compressed);
}

void ObjectBankWriter::WriteTOCEntries(const String &name, uint8 pos, const SoundObject& object)
{
	this->WriteTOCEntry(name, pos, ObjectType::Sound, this->model.GetSupportedResourceVersions().maxSoundVersion, ObjectStreamFormat::Compressed);
}

//Private methods
ChunkVersion ObjectBankWriter::DeterminePerformanceVersion(uint8 majorVersion) const
{
	majorVersion = Math::Min(majorVersion, this->model.GetSupportedResourceVersions().maxPerformanceVersion.major);

	switch(majorVersion)
	{
		case 0:
			return {0, 3};
		case 1:
			return {1, 0};
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void ObjectBankWriter::WritePCMData(const AbstractSample& abstractSample, const ChunkVersion& dataVersion)
{
	const auto* encryptedSample = dynamic_cast<const EncryptedSample *>(&abstractSample);
	if(encryptedSample)
	{
		encryptedSample->Read()->FlushTo(this->outputStream);
		return;
	}

	const auto& sampleObject = dynamic_cast<const SampleObject &>(abstractSample);

	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);
	UniquePointer<PCMFormatWriter> bankObjectFormatWriter = CreatePCMWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(sampleObject.data);
	bufferedOutputStream.Flush();
}

void ObjectBankWriter::WriteSTS(const SingleTouchSettings &singleTouchSettings, const ChunkVersion& dataVersion)
{
	UniquePointer<Compressor> compressor = this->BeginWritingObjectData();

	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();
	UniquePointer<PerformanceFormatWriter> bankObjectFormatWriter = CreatePerformanceWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(singleTouchSettings);

	buffer.CreateInputStream()->FlushTo(*compressor);
	compressor->Finalize();

	this->EndWritingObject();
}

void ObjectBankWriter::WriteStyle(const StyleObject &style, const ChunkVersion& dataVersion)
{
	UniquePointer<Compressor> compressor = this->BeginWritingObjectData();

	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();
	UniquePointer<StyleFormatWriter> bankObjectFormatWriter = CreateStyleWriter(*outputStream, dataVersion);
	bankObjectFormatWriter->Write(style.data);

	buffer.CreateInputStream()->FlushTo(*compressor);
	compressor->Finalize();

	this->EndWritingObject();
}

void ObjectBankWriter::WriteTOCEntry(const String& name, uint8 pos, ObjectType objectType, const ChunkVersion& version, ObjectStreamFormat streamFormat)
{
	HeaderEntry headerEntry;
	headerEntry.name = name;
	headerEntry.pos = pos;
	headerEntry.type = objectType;
	headerEntry.dataVersion = version;
	this->WriteIndexEntry(headerEntry, streamFormat);
	this->objectVersionMap[pos][objectType] = version;
}
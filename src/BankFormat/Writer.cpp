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
#include "../Writer/PerformanceWriter.hpp"
#include "../Writer/StyleWriter.hpp"
#include "../PCMFormat/PCMFormatWriter.hpp"
#include "../PCMFormat/PCMWriterFactory.hpp"
#include <libkorg/BankFormat/EncryptedSample.hpp>
#include <libkorg/BankFormat/SampleObject.hpp>
//Namespaces
using namespace BankFormat;

//Public methods
void Writer::Write(const ObjectBank<AbstractSample> &bank)
{
	this->WriteHeader();

	this->BeginCrossReferencedChunk(ChunkType::ObjectTOC, 0, 0, ChunkHeaderFlags::Leaf);
	for(const auto& objectEntry : bank.Objects())
	{
		this->WriteTOCEntry(objectEntry.name, objectEntry.pos, ObjectType::PCM, this->DeterminePCMVersion(*objectEntry.object));
	}
	this->EndChunk();

	for(const auto& objectEntry : bank.Objects())
	{
		this->WritePCM(*objectEntry.object);
	}

	this->WriteCrossReferenceTable();

	this->EndChunk();
}

void Writer::Write(const ObjectBank<FullStyle> &bank)
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
}

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

void Writer::WritePCM(const AbstractSample& abstractSample)
{
	const EncryptedSample* encryptedSample = dynamic_cast<const EncryptedSample *>(&abstractSample);
	if(encryptedSample)
		NOT_IMPLEMENTED_ERROR; //TODO: version for encrypted should be saved

	const auto& sampleObject = dynamic_cast<const SampleObject &>(abstractSample);

	ChunkVersion dataVersion = this->DeterminePCMVersion(abstractSample);
	this->BeginCrossReferencedChunk(ChunkType::PCMData, dataVersion.major, dataVersion.minor, ChunkHeaderFlags::UnknownAlwaysSetInBankFile);

	BufferedOutputStream bufferedOutputStream(this->outputStream);
	DataWriter dataWriter(true, bufferedOutputStream);
	UniquePointer<PCMFormatWriter> bankObjectFormatWriter = CreatePCMWriter(dataWriter, dataVersion);
	bankObjectFormatWriter->Write(sampleObject.data);
	bufferedOutputStream.Flush();

	this->EndChunk();
}

void Writer::WriteSTS(const SingleTouchSettings &singleTouchSettings)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();

	PerformanceWriter performanceWriter(*outputStream);
	performanceWriter.Write(singleTouchSettings);

	this->BeginCrossReferencedChunk(ChunkType::PerformancesData, singleTouchSettings.Version().major, singleTouchSettings.Version().minor, ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
	buffer.CreateInputStream()->FlushTo(this->outputStream);
	this->EndChunk();
}

void Writer::WriteStyle(const StyleObject &style)
{
	DynamicByteBuffer buffer;
	UniquePointer<SeekableOutputStream> outputStream = buffer.CreateOutputStream();
	StyleWriter styleWriter(*outputStream);

	styleWriter.Write(style);

	this->BeginCrossReferencedChunk(ChunkType::StyleObject, 0, 0, ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
	buffer.CreateInputStream()->FlushTo(this->outputStream);
	this->EndChunk();
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
}
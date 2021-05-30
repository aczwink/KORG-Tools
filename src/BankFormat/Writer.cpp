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
#include <libkorg/BankFormat/Writer.hpp>
//Namespaces
using namespace libKORG::BankFormat;
using namespace StdXX;

//Public methods
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

void Writer::WriteIndexEntry(const HeaderEntry &headerEntry)
{
	TextWriter textWriter(this->outputStream, TextCodecType::ASCII);

	textWriter.WriteFixedLengthString(headerEntry.name, HEADERENTRY_NAME_SIZE);
	dataWriter.WriteByte(static_cast<byte>(headerEntry.type));
	dataWriter.WriteByte(0);
	dataWriter.WriteByte(headerEntry.pos);
	dataWriter.WriteByte(headerEntry.dataVersion.major);
	dataWriter.WriteByte(headerEntry.dataVersion.minor);
	dataWriter.WriteByte(0);

	this->headerEntries.Push(headerEntry);
}

//Private methods
libKORG::ChunkHeaderFlags Writer::IsLeaf(ObjectType objectType) const
{
	switch(objectType)
	{
		case ObjectType::MultiSample:
		case ObjectType::PCM:
		case ObjectType::Sound:
			return ChunkHeaderFlags::Leaf;
	}

	return ChunkHeaderFlags::UnknownAlwaysSetInBankFile;
}

ChunkType Writer::MapObjectTypeToDataType(ObjectType objectType) const
{
	switch(objectType)
	{
		case ObjectType::Performance:
		case ObjectType::StylePerformances:
			return ChunkType::PerformancesData;
		case ObjectType::Style:
			return ChunkType::StyleObject;
		case ObjectType::Sound:
			return ChunkType::SoundData;
		case ObjectType::MultiSample:
			return ChunkType::MultiSampleData;
		case ObjectType::PCM:
			return ChunkType::PCMData;
		case ObjectType::PAD:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		case ObjectType::SongBookEntry:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		case ObjectType::SongBook:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	RAISE(ErrorHandling::IllegalCodePathError);
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
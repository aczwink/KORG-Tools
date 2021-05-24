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
#pragma once
//Local
#include <libkorg/FullStyle.hpp>
#include <libkorg/BankFormat/ObjectBank.hpp>
#include <libkorg/BankFormat/BankFormat.hpp>
#include <libkorg/BankFormat/SoundObject.hpp>
#include <libkorg/Model.hpp>
#include <libkorg/BankFormat/MultiSamplesObject.hpp>
#include "../Writer/ChunkWriter.hpp"
//Namespaces
using namespace StdXX;
using namespace libKORG;

namespace libKORG::BankFormat
{
	class Writer : public ChunkWriter
	{
	public:
		//Constructor
		inline Writer(SeekableOutputStream &outputStream, const Model& model) : ChunkWriter(outputStream),
															fourccWriter(false, outputStream), model(model)
		{
		}

		//Methods
		template<typename T>
		void Write(const ObjectBank<T> &bank);
		void Write(const MultiSamplesObject& multiSamplesObject);

	private:
		//Members
		DataWriter fourccWriter;
		DynamicArray<uint32> crossReferenceObjects;
		BinaryTreeMap<uint8, BinaryTreeMap<ObjectType, ChunkVersion>> objectVersionMap;
		const Model& model;

		//Methods
		ChunkVersion DeterminePCMVersion(const AbstractSample& sample) const;
		void WriteCrossReferenceTable();
		void WriteHeader();
		void WriteObjects(uint8 pos, const AbstractSample& sampleObject);
		void WriteObjects(uint8 pos, const FullStyle& fullStyle);
		void WriteObjects(uint8 pos, const SoundObject& soundObject);
		void WritePCMData(const AbstractSample& sample, const ChunkVersion& dataVersion);
		void WriteSoundData(const SoundObject& soundObject, const ChunkVersion& dataVersion);
		void WriteSTS(const SingleTouchSettings &singleTouchSettings, const ChunkVersion& dataVersion);
		void WriteStyle(const StyleObject &style, const ChunkVersion& dataVersion);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const AbstractSample& object);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const FullStyle& object);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const SoundObject& object);
		void WriteTOCEntry(const String &name, uint8 pos, libKORG::BankFormat::ObjectType objectType, const ChunkVersion& version);

		//Inline
		inline void BeginChunk(ChunkType type, uint8 versionMajor, uint8 versionMinor, ChunkHeaderFlags flag)
		{
			ChunkWriter::BeginChunk((uint8)type, versionMajor, versionMinor, flag | ChunkHeaderFlags::UnknownAlwaysSetInBankFile);
		}

		inline void BeginCrossReferencedChunk(ChunkType type, uint8 versionMajor, uint8 versionMinor, ChunkHeaderFlags flag)
		{
			this->crossReferenceObjects.Push(this->outputStream.QueryCurrentOffset());
			this->BeginChunk(type, versionMajor, versionMinor, flag);
		}
	};
}
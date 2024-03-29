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
#include <libkorg/BankFormat/Writer.hpp>
#include "libkorg/ChunkFormat/ChunkWriter.hpp"
//Namespaces
using namespace StdXX;
using namespace libKORG;

namespace libKORG::BankFormat
{
	class ObjectBankWriter : public Writer
	{
	public:
		//Constructor
		inline ObjectBankWriter(SeekableOutputStream &outputStream, const Model& model) : Writer(outputStream),
			model(model)
		{
		}

		//Methods
		void Write(const MultiSamplesObject& multiSamplesObject);
		void WriteObjects(uint8 pos, const AbstractSample& sampleObject);
		void WriteObjects(uint8 pos, const FullStyle& fullStyle);
		void WriteObjects(uint8 pos, const PerformanceObject& performanceObject);
		void WriteObjects(uint8 pos, const SoundObject& soundObject);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const AbstractSample& object);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const FullStyle& object);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const PerformanceObject& object);
		void WriteTOCEntries(const StdXX::String& name, uint8 pos, const SoundObject& object);

	private:
		//Members
		BinaryTreeMap<uint8, BinaryTreeMap<ObjectType, ChunkVersion>> objectVersionMap;
		const Model& model;

		//Methods
		ChunkVersion DeterminePerformanceVersion(uint8 majorVersion) const;
		void WritePCMData(const AbstractSample& sample, const ChunkVersion& dataVersion);
		void WriteSTS(const SingleTouchSettings &singleTouchSettings, const ChunkVersion& dataVersion);
		void WriteStyle(const StyleObject &style, const ChunkVersion& dataVersion);
		void WriteTOCEntry(const String &name, uint8 pos, libKORG::BankFormat::ObjectType objectType, const ChunkVersion& version, ObjectStreamFormat streamFormat);
	};
}
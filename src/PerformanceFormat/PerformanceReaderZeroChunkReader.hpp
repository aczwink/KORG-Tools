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
//Local
#include <libkorg/BankFormat/BankObject.hpp>
#include "Format1.0/PerformanceFormat1_0VReader.hpp"
#include "Format2.1/PerformanceFormat2_1VReader.hpp"
#include "Format2.0/PerformanceFormat2_0VReader.hpp"
#include "Format0.3/PerformanceFormatV0_3Reader.hpp"
#include "Format0.2/PerformanceFormatV0_2Reader.hpp"

class PerformanceReaderZeroChunkReader : public BankFormat::BankObjectReader
{
public:
	//Methods
	BankFormat::BankObject *TakeResult() override
	{
		return this->formatReader->TakeResult();
	}

	//Functions
	static BankObjectReader* CreateInstance(bool stylePerformances, const ChunkVersion& chunkVersion)
	{
		switch (chunkVersion.AsUInt16())
		{
			case 0x0002:
				return new PerformanceFormatV0_2Reader(stylePerformances);
			case 0x0003:
				return new PerformanceFormatV0_3Reader(stylePerformances);
		}

		UniquePointer<BankFormat::BankObjectReader> formatReader = CreateFormatReader(chunkVersion);
		if(formatReader.IsNull())
			return nullptr;
		return new PerformanceReaderZeroChunkReader(stylePerformances, Move(formatReader));
	}

protected:
	//Methods
	ChunkReader* OnEnteringChunk(const ChunkHeader &chunkHeader) override
	{
		if(chunkHeader.id == 0)
			return this->formatReader.operator->();
		return nullptr;
	}

	void ReadDataChunk(const ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override
	{
	}

private:
	//Members
	bool stylePerformances;
	UniquePointer<BankFormat::BankObjectReader> formatReader;

	//Constructor
	inline PerformanceReaderZeroChunkReader(bool stylePerformances, UniquePointer<BankFormat::BankObjectReader>&& formatReader) : stylePerformances(stylePerformances), formatReader(Move(formatReader))
	{
	}

	//Functions
	static UniquePointer<BankFormat::BankObjectReader> CreateFormatReader(const ChunkVersion& chunkVersion)
	{
		switch(chunkVersion.AsUInt16())
		{
			case 0x0100:
				return new PerformanceFormat1_0VReader;
			case 0x0200:
				return new PerformanceFormat2_0VReader;
			case 0x0201:
				return new PerformanceFormat2_1VReader;
		}

		return nullptr;
	}
};
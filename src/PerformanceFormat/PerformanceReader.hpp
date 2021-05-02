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
#include "Format1.0/PerformanceFormat1_0V0_0Reader.hpp"
#include "PerformanceFormatReader.hpp"

class PerformanceReader : public BankFormat::BankObjectReader
{
public:
	//Methods
	BankFormat::BankObject *TakeResult() override
	{
		if(this->stylePerformances)
			return this->formatReader->TakeSTSResult();
		return this->formatReader->TakePerformanceResult();
	}

	//Functions
	static PerformanceReader* CreateInstance(bool stylePerformances, const ChunkVersion& chunkVersion)
	{
		UniquePointer<PerformanceFormatReader> formatReader = CreateFormatReader(chunkVersion);
		if(formatReader.IsNull())
			return nullptr;
		return new PerformanceReader(stylePerformances, Move(formatReader));
	}

protected:
	//Methods
	ChunkReader* OnEnteringChunk(const ChunkHeader &chunkHeader) override
	{
		return this->formatReader.operator->();
	}

	void ReadDataChunk(const ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

private:
	//Members
	bool stylePerformances;
	UniquePointer<PerformanceFormatReader> formatReader;

	//Constructor
	inline PerformanceReader(bool stylePerformances, UniquePointer<PerformanceFormatReader>&& formatReader) : stylePerformances(stylePerformances), formatReader(Move(formatReader))
	{
	}

	//Functions
	static UniquePointer<PerformanceFormatReader> CreateFormatReader(const ChunkVersion& chunkVersion)
	{
		switch(chunkVersion.AsUInt16())
		{
			case 0x0100:
				return new PerformanceFormat1_0V0_0Reader;
		}

		return nullptr;
	}
};
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
#include "../PerformanceFormatReader.hpp"

class PerformanceFormatV0_3Reader : public PerformanceFormatReader, public libKORG::BankFormat::BankObjectReader
{
public:
	//Constructor
	inline PerformanceFormatV0_3Reader(bool stylePerformances) : stylePerformances(stylePerformances)
	{
	}

	//Methods
	libKORG::BankFormat::BankObject *TakeResult() override;

protected:
	//Methods
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override;
	void ReadDataChunk(const libKORG::ChunkHeader& chunkHeader, StdXX::DataReader &dataReader) override;

private:
	//Members
	bool stylePerformances;

	//Methods
	void Read0x00010008Chunk(StdXX::DataReader &dataReader);
	void Read0x02000108Chunk(StdXX::DataReader &dataReader);
	void ReadEffectGroup(StdXX::DataReader &dataReader);
	void ReadTrackParams(StdXX::DataReader &dataReader);
};
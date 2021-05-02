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
#include "ChunkFormat.hpp"

namespace libKORG
{
	class ChunkReader
	{
	public:
		//Methods
		void ReadData(StdXX::InputStream& inputStream);

	protected:
		//Abstract
		virtual ChunkReader* OnEnteringChunk(const ChunkHeader& chunkHeader) = 0;
		virtual void ReadDataChunk(const ChunkHeader& chunkHeader, StdXX::DataReader& dataReader) = 0;

		//Overrideable
		virtual bool IsDataChunk(const ChunkHeader& chunkHeader);
		virtual void OnLeavingChunk(const ChunkHeader& chunkHeader);

		//Inline
		inline uint32 ReadFourCC(StdXX::InputStream& inputStream)
		{
			return this->CreateFourCCReader(inputStream).ReadUInt32();
		}

	private:
		//Methods
		ChunkHeader ReadChunkHeader(StdXX::DataReader& dataReader);
		void ReadChunks(StdXX::InputStream& inputStream);

		//Inline
		inline StdXX::DataReader CreateFourCCReader(StdXX::InputStream &inputStream)
		{
			return StdXX::DataReader(false, inputStream);
		}
	};
}
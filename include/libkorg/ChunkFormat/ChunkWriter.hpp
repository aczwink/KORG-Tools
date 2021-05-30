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

namespace libKORG {
	class ChunkWriter
	{
	public:
		//Constructor
		inline ChunkWriter(StdXX::SeekableOutputStream &outputStream) : outputStream(outputStream),
			dataWriter(true, outputStream)
		{
		}

		//Inline
		inline void BeginChunk(uint8 type, uint8 versionMajor, uint8 versionMinor, uint8 flags)
		{
			this->dataWriter.WriteByte(type);
			this->dataWriter.WriteByte(versionMajor);
			this->dataWriter.WriteByte(versionMinor);
			this->dataWriter.WriteByte(flags);
			this->dataWriter.WriteUInt32(0);

			this->BeginSizeBracket();
		}

		inline void BeginChunk(uint8 type, uint8 versionMajor, uint8 versionMinor, libKORG::ChunkHeaderFlags flag)
		{
			this->BeginChunk(type, versionMajor, versionMinor, (uint8) flag);
		}

		inline void BeginSizeBracket16()
		{
			this->dataWriter.WriteUInt16(0);
			this->BeginSizeBracket();
		}

		inline void EndChunk()
		{
			uint64 offset = this->outputStream.QueryCurrentOffset();
			uint64 chunkDataOffset = this->chunkDataOffsets.Pop();
			this->outputStream.SeekTo(chunkDataOffset - 4);
			this->dataWriter.WriteUInt32(offset - chunkDataOffset);
			this->outputStream.SeekTo(offset);
		}

		inline void EndSizeBracket16()
		{
			uint64 offset = this->outputStream.QueryCurrentOffset();
			uint64 chunkDataOffset = this->chunkDataOffsets.Pop();
			this->outputStream.SeekTo(chunkDataOffset - 2);
			this->dataWriter.WriteUInt16(offset - chunkDataOffset);
			this->outputStream.SeekTo(offset);
		}

	protected:
		//Members
		StdXX::SeekableOutputStream &outputStream;
		StdXX::DataWriter dataWriter;

	private:
		//Members
		StdXX::DynamicArray<uint64> chunkDataOffsets;

		//Inline
		inline void BeginSizeBracket()
		{
			this->chunkDataOffsets.Push(outputStream.QueryCurrentOffset());
		}
	};
}
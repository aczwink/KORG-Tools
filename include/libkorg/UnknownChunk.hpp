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
#pragma once
//Local
#include "ChunkFormat.hpp"

namespace libKORG
{
	class UnknownChunk
	{
	public:
		//Constructors
		inline UnknownChunk()
		{
		}

		inline UnknownChunk(const ChunkHeader& header, StdXX::InputStream& stream) : header(header)
		{
			auto outStream = this->buffer.CreateOutputStream();
			stream.FlushTo(*outStream);
		}

		UnknownChunk(const UnknownChunk&) = default;

		//Operators
		UnknownChunk& operator=(const UnknownChunk&) = default;

		//Properties
		inline const auto& Buffer() const
		{
			return this->buffer;
		}

		inline const ChunkHeader& Header() const
		{
			return this->header;
		}

	private:
		//Members
		ChunkHeader header;
		StdXX::DynamicByteBuffer buffer;
	};
}
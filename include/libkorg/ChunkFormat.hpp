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
#include <StdXX.hpp>

namespace libKORG
{
	struct ChunkVersion
	{
		uint8 major;
		uint8 minor;

		//Operators
		inline bool operator<(const ChunkVersion& rhs) const
		{
			if(this->major < rhs.major)
				return true;
			return this->minor < rhs.minor;
		}

		inline bool operator>(const ChunkVersion& rhs) const
		{
			return rhs < *this;
		}

		//Inline
		inline uint16 AsUInt16() const
		{
			return (this->major << 8) | this->minor;
		}
	};

	struct ChunkHeader
	{
		uint32 id;
		uint32 size;

		uint8 type;
		uint8 flags;
		ChunkVersion version;
	};

	enum class ChunkHeaderFlags
			{
		/**
		 * Only set for the very first chunk of a bank format file
		 */
		Unknown = 0x4,
		/**
		 * A leaf has only data and no children.
		 * A non-leaf has only children and no data itself
		 */
		Leaf = 0x8,
		/**
		 * Always set in bank files for every chunk but not the ones in the nested objects (i.e. styles etc.)
		 */
		UnknownAlwaysSetInBankFile = 0x10,
		OC31Compressed = 0x20,
		Encrypted = 0x40,
	};

	inline uint8 operator|(ChunkHeaderFlags a, ChunkHeaderFlags b)
	{
		return (uint8) a | (uint8) b;
	}
}
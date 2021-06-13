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
#include "libkorg/ChunkFormat/ChunkReader.hpp"

namespace libKORG::BankFormat
{
	class BankObject
	{
	public:
		virtual ~BankObject() = default;
	};

	class BankObjectReader : public ChunkReader
	{
	public:
		//Destructor
		virtual ~BankObjectReader() = default;

		//Abstract
		virtual BankObject* TakeResult() = 0;
	};
}

namespace libKORG
{
	class AbstractSample : public BankFormat::BankObject
	{
	public:
		//Abstract
		virtual bool Equals(const AbstractSample& other) const = 0;
		virtual uint64 GetId() const = 0;
		virtual uint32 GetSize() const = 0;

		//Operators
		inline bool operator==(const AbstractSample& other) const
		{
			return this->Equals(other);
		}
	};
}
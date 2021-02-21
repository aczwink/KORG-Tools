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
#include "BankObject.hpp"

namespace libKORG
{
	class Sample : public AbstractSample
	{
	public:
		//Constructor
		inline Sample(StdXX::InputStream& inputStream)
		{
			this->ReadData(inputStream);
		}

		//Methods
		void WriteUnknownData(StdXX::OutputStream& outputStream) const;

	private:
		//Members
		uint64 unknown;
		uint32 sampleRate;
		byte unknown2[48];
		StdXX::DynamicArray<uint16> samples;

		//Methods
		void ReadData(StdXX::InputStream& inputStream);
	};
}
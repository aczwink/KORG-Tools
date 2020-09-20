/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "StyleBankObject.hpp"

class Performance : public StyleBankObject
{
public:
	//Constructor
	inline Performance(DataReader& dataReader, uint32 dataSize) : data(dataSize)
	{
		this->ReadData(dataReader, dataSize);
	}

	void WriteData(DataWriter &dataWriter) const override
	{
		dataWriter.WriteBytes(this->data.Data(), this->data.Size());
	}

private:
	//Members
	FixedSizeBuffer data;

	//Inline
	inline void ReadData(DataReader& dataReader, uint32 dataSize)
	{
		dataReader.ReadBytes(this->data.Data(), dataSize);
	}
};
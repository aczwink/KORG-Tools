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
#include <StdXX.hpp>
using namespace StdXX;
#include "StyleBankObject.hpp"

class Style : public StyleBankObject
{
public:
	//Constructor
	inline Style(const String& name, StdXX::InputStream& inputStream) : name(name)
	{
		this->ReadData(inputStream);
	}

	//Properties
	inline const String& Name() const
	{
		return this->name;
	}

	//Methods
	void WriteData(DataWriter &dataWriter) const override;

private:
	//Members
	String name;

	//Methods
	void ReadData(StdXX::InputStream& inputStream);
};
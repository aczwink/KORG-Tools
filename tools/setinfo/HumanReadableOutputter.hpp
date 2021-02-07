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
#include "FormattedOutputter.hpp"

class HumanReadableOutputter : public FormattedOutputter
{
public:
	//Constructor
	inline HumanReadableOutputter(TextWriter &output) : FormattedOutputter(output)
	{
		this->depth = 0;
	}

	void BeginSection(const String &name) override
	{
		this->OutputTabs();
		this->output << name << u8":" << endl;
		this->depth++;
	}

	void EndSection() override
	{
		this->depth--;
	}

	void OutputProperty(const String &name, const String &value) override
	{
		this->OutputTabs();
		this->output << name << u8": " << value << endl;
	}

	void OutputProperty(const String &name, int8 value) override
	{
		this->OutputProperty(name, String::Number(value));
	}

	void OutputProperty(const String &name, uint8 value) override
	{
		this->OutputProperty(name, uint16(value));
	}

	void OutputProperty(const String &name, uint16 value) override
	{
		if(name.StartsWith(u8"unknown"))
			this->OutputProperty(name, String::Number(value) + u8" / " + String::HexNumber(value));
		else
			this->OutputProperty(name, String::Number(value));
	}

private:
	//Members
	uint8 depth;

	//Inline
	inline void OutputTabs()
	{
		this->output.WriteTabs(this->depth);
	}
};
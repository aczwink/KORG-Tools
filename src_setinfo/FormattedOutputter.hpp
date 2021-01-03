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
class FormattedOutputter
{
public:
	//Constructor
	inline FormattedOutputter(StdXX::TextWriter& output) : output(output)
	{
	}

	//Abstract
	virtual void BeginSection(const String& name) = 0;
	virtual void EndSection() = 0;
	virtual void OutputProperty(const String& name, int8 value) = 0;
	virtual void OutputProperty(const String& name, uint8 value) = 0;
	virtual void OutputProperty(const String& name, uint16 value) = 0;
	virtual void OutputProperty(const String& name, const String& value) = 0;

protected:
	//Members
	StdXX::TextWriter& output;
};

class Section
{
public:
	//Constructor
	inline Section(const String& name, FormattedOutputter& outputter) : name(name), outputter(outputter)
	{
		outputter.BeginSection(name);
	}

	//Destructor
	inline ~Section()
	{
		outputter.EndSection();
	}

private:
	//Members
	String name;
	FormattedOutputter& outputter;
};
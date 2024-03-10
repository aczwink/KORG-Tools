/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;

class FormattedOutputter
{
public:
	//Constructor
	inline FormattedOutputter(StdXX::TextWriter& output) : output(output)
	{
	}

	//Destructor
	virtual ~FormattedOutputter() = default;

	//Abstract
	virtual void BeginSection(const String& name) = 0;
	virtual void EndSection() = 0;
	virtual void OutputProperty(const String& name, int32 value) = 0;
	virtual void OutputProperty(const String& name, const String& value) = 0;

	//Inline
	template <typename T, typename U = Type::EnableIf_t<Type::IsSameType<T, bool>::type> >
	inline void OutputProperty(const String& name, T value)
	{
		String string = value ? u8"true" : u8"false";
		this->OutputProperty(name, string);
	}

	inline void OutputProperty(const String& name, int8 value)
	{
		this->OutputProperty(name, int16(value));
	}

	inline void OutputProperty(const String& name, int16 value)
	{
		this->OutputProperty(name, int32(value));
	}

	inline void OutputProperty(const String& name, uint8 value)
	{
		this->OutputProperty(name, uint16(value));
	}

	inline void OutputProperty(const String& name, uint16 value)
	{
		this->OutputProperty(name, uint32(value));
	}

	inline void OutputProperty(const String& name, uint32 value)
	{
		if(name.StartsWith(u8"unknown"))
			this->OutputProperty(name, String::Number(value) + u8" / " + String::HexNumber(value));
		else
			this->OutputProperty(name, String::Number(value));
	}

	inline void OutputProperty(const String& name, const libKORG::Pitch& value)
	{
		this->OutputProperty(name, value.ToString());
	}

	inline void OutputUnknownProperties(const DynamicByteBuffer& buffer)
	{
		for(uint32 i = 0; i < buffer.Size(); i++)
		{
			this->OutputProperty(u8"unknown[" + String::Number(i) + u8"]", buffer[i]);
		}
	}

	inline void OutputUnknownProperties(const byte* buffer, uint32 bufferSize)
	{
		for(uint32 i = 0; i < bufferSize; i++)
		{
			this->OutputProperty(u8"unknown[" + String::Number(i) + u8"]", buffer[i]);
		}
	}

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
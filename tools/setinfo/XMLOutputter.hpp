/*
 * Copyright (c) 2021-2026 Amir Czwink (amir130@hotmail.de)
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

class XMLOutputter : public FormattedOutputter
{
public:
	//Constructor
	inline XMLOutputter(TextWriter &output) : FormattedOutputter(output), xmlSerializer(output.Stream())
	{
	}

	void BeginSection(const String &name) override
	{
		this->xmlSerializer.EnterElement(this->CreateValidName(name));
	}

	void EndSection() override
	{
		this->xmlSerializer.LeaveElement();
	}

	void OutputProperty(const String &name, const String &value) override
	{
		this->xmlSerializer << Serialization::Binding(this->CreateValidName(name), value);
	}

	void OutputProperty(const String &name, int32 value) override
	{
		this->OutputProperty(name, String::Number(value));
	}

private:
	Serialization::XMLSerializer xmlSerializer;

	//Inline
	inline String CreateValidName(const String& name)
	{
		String changed = name;
		if(IsDecimalDigitChar(*changed.begin()))
		{
			changed = u8"_" + changed;
		}
		return changed.Replace(u8" ", u8"")
			.Replace(u8"#", u8"")
			.Replace(u8"[", u8"")
			.Replace(u8"]", u8"")
			.Replace(u8"/", u8"")
			.Replace(u8"?", u8"_unknown_");
	}
};
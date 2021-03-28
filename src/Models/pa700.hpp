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

class Pa700 : public libKORG::Model
{
public:
	StdXX::String GetCustomization() const override
	{
		return u8"STD";
	}

	StdXX::String GetMachId() const override
	{
		return u8"111A";
	}

	libKORG::ChunkVersion GetMaximumPerformanceVersion() const override
	{
		return {2, 1};
	}

	StdXX::String GetName() const override
	{
		return u8"Pa700";
	}
};

class Pa700OR : public Pa700
{
public:
	StdXX::String GetCustomization() const override
	{
		return u8"ORT";
	}

	StdXX::String GetMachId() const override
	{
		return u8"113A";
	}

	StdXX::String GetName() const override
	{
		return u8"Pa700Oriental";
	}
};
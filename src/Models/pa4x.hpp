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

class Pa4X : public libKORG::Model
{
public:
	bool IsSampleCompressionSupported() const override
	{
		return true;
	}

	BankSetup GetBankSetup() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	StdXX::String GetCustomization() const override
	{
		return u8"STD";
	}

	StdXX::String GetMachId() const override
	{
		return u8"110A";
	}

	StdXX::String GetName() const override
	{
		return u8"Pa4X";
	}

	uint32 GetSampleRAMSize() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return 0;
	}

	SupportedResourceVersions GetSupportedResourceVersions() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return {
		};
	}
};

class Pa4XORT : public Pa4X
{
public:
	StdXX::String GetCustomization() const override
	{
		return u8"ORT";
	}

	StdXX::String GetName() const override
	{
		return u8"Pa4XOriental";
	}
};
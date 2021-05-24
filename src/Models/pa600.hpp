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

class Pa600 : public libKORG::Model
{
public:
	bool IsSampleCompressionSupported() const override
	{
		return false;
	}

	BankSetup GetBankSetup() const override
	{
		return {
			.factoryBankIds = {1, 15},
			.userBankIds = {18, 20},
			.favoriteBankIds = {{21, 32}},
			.localBankIds = {},
			.nStylesPerBank = 40,
		};
	}

	StdXX::String GetCustomization() const override
	{
		return u8"STD";
	}

	StdXX::String GetMachId() const override
	{
		return u8"103A";
	}

	StdXX::String GetName() const override
	{
		return u8"Pa600";
	}

	uint32 GetSampleRAMSize() const override
	{
		return 96;
	}

	SupportedResourceVersions GetSupportedResourceVersions() const override
	{
		return {
			{3, 0},
			{0, 0},
			{1, 0},
			{3, 0},
			{0, 0}
		};
	}
};

class Pa600QT : public Pa600
{
public:
	StdXX::String GetCustomization() const override
	{
		return u8"QTK";
	}

	StdXX::String GetName() const override
	{
		return u8"Pa600QT";
	}
};
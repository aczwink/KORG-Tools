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
	bool IsSampleCompressionSupported() const override
	{
		return true;
	}

	BankSetup GetBankSetup() const override
	{
		return {
			.factoryBankIds = {1, 12},
			.userBankIds = {13, 24},
			.favoriteBankIds = {},
			.localBankIds = {},
			.nStylesPerBank = 48,
		};
	}

	StdXX::String GetCustomization() const override
	{
		return u8"STD";
	}

	StdXX::String GetMachId() const override
	{
		return u8"111A";
	}

	StdXX::String GetName() const override
	{
		return u8"Pa700";
	}

	uint32 GetSampleRAMSize() const override
	{
		return 256;
	}

	SupportedResourceVersions GetSupportedResourceVersions() const override
	{
		return {
			{3, 0},
			{0, 0},
			{2, 1},
			{3, 0}
		};
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
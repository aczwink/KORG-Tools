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
		return {
			.padBanks = {
				.factoryBankIds = {255, 255}, //TODO: check this
				.localBankIds = {{31, 40}},
				.userBankIds = {255, 255} //TODO: check this
			},
			.performanceBanks = {
				.factoryBankIds = {1, 11},
				.userBankIds = {{11, 17}}
			},
			.soundBanks = {
				.nUserBanks = 0 //TODO:
			},
			.styleBanks = {
				.factoryBankIds = {0, 0}, //TODO:
				.userBankIds = {0, 0}, //TODO:
				.favoriteBankIds = {}, //TODO:
				.localBankIds = {}, //TODO:
				.nStylesPerBank = {}, //TODO:
			}
		};
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
		return 1024 * 3 / 2; //"1,5 GB" is reported by KORG
	}

	SupportedResourceVersions GetSupportedResourceVersions() const override
	{
		return {
			{3, 0},
			{0, 0},
			{2, 1},
			{3, 0},
			{0, 0}
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
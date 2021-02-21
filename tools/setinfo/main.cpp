/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg.hpp>
#include "HumanReadableOutputter.hpp"
#include "StyleOutputter.hpp"
#include "PerformanceOutputter.hpp"
#include "XMLOutputter.hpp"

using namespace libKORG;

struct PrintSettings
{
	bool showObjects;
	Optional<uint8> bankNumber;
	Optional<uint8> posNumber;
};

template<typename BankType, typename ObjectOutputter>
void PrintBanks(const PrintSettings& printSettings, const Map<uint8, BankType>& banks, FormattedOutputter& outputter, Function<String(uint8)> bankNumberToString)
{
	uint8 bankNumber = 0;
	for(const auto& kv : banks)
	{
		if(printSettings.bankNumber.HasValue() && (*printSettings.bankNumber != bankNumber++))
			continue;

		Section bankSection(bankNumberToString(kv.key), outputter);

		uint8 posNumber = 0;
		for(const auto& kv2 : kv.value.Objects())
		{
			if(printSettings.posNumber.HasValue() && (*printSettings.posNumber != posNumber++))
				continue;

			Section objectSection(BankPositionToString(kv2.key) + u8" - " + kv2.value.template Get<0>(), outputter);

			if(printSettings.showObjects)
			{
				ObjectOutputter objectOutputter(outputter);
				objectOutputter.Output(*kv2.value.template Get<1>());
			}
		}
	}
}

void PrintPerformanceBanks(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	PrintBanks<PerformanceBank, PerformanceOutputter>(printSettings, set.PerformanceBanks(), outputter, PerformanceBankNumberToString);
}

void PrintStyleBanks(const PrintSettings& printSettings, Set& set, FormattedOutputter& outputter)
{
	PrintBanks<StyleBank, StyleOutputter>(printSettings, set.StyleBanks(), outputter, StyleBankNumberToString);
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	FileSystem::Path setPath = FileSystem::FileSystemsManager::Instance().OSFileSystem().FromNativePath(args[0]);
	Set set(setPath);

	bool showPerformances = false;
	bool showStyles = true;

	PrintSettings printSettings;
	printSettings.showObjects = true;
	printSettings.bankNumber = 0;
	printSettings.posNumber = 0;

	UniquePointer<FormattedOutputter> outputter = new XMLOutputter(stdOut);

	if(showPerformances)
		PrintPerformanceBanks(printSettings, set, *outputter);
	if(showStyles)
		PrintStyleBanks(printSettings, set, *outputter);

	return EXIT_SUCCESS;
}

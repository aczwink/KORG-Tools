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

using namespace libKORG;

int32 Main(const String &programName, const FixedArray<String> &args)
{
	FileSystem::Path setPath = FileSystem::OSFileSystem::GetInstance().FromNativePath(args[0]);
	Set set(setPath);

	UniquePointer<FormattedOutputter> outputter = new HumanReadableOutputter(stdOut);

	for(const auto& kv : set.StyleBanks())
	{
		Section bankSection(StyleBankNumberToString(kv.key), *outputter);

		for(const auto& kv2 : kv.value.Objects())
		{
			Section objectSection(BankPositionToString(kv2.key) + u8" - " + kv2.value.Get<0>(), *outputter);

			StyleOutputter styleOutputter(*outputter);
			styleOutputter.Output(*kv2.value.Get<1>());
		}
	}

	/*

	FileSystem::Path outPath = String(u8"/home/amir/Desktop/korg/_OUT/");
	for(const auto& kv : set.SampleBanks())
	{
		for(const auto& kv2 : kv.value.Objects())
		{
			if(kv2.value.Get<1>().IsInstanceOf<Sample>())
			{
				const Sample& sample = dynamic_cast<const Sample &>(*kv2.value.Get<1>());

				FileOutputStream fileOutputStream(outPath / String(u8"PCM") / kv2.value.Get<0>(), true);
				sample.WriteUnknownData(fileOutputStream);
			}
		}
	}

	 */

	/*
	uint32 nPerformances = 0;
	uint32 nStyles = 0;
	stdOut << u8"Styles:" << endl;
	for(const SharedPointer<BankObject>& object : styleBank.Objects())
	{
		if(object.IsInstanceOf<Style>())
		{
			const Style& style = dynamic_cast<const Style&>(*object);
			uint8 pos = styleBank.GetPositionOf(style);

			stdOut << style.Name()
			nStyles++;
		}
		else
			nPerformances++;
	}
	stdOut << u8"Total style count: " << nStyles << endl;
	stdOut << u8"Performances: " << nPerformances << endl;

	ObjectBank filtered;
	for(const SharedPointer<BankObject>& object : styleBank.Objects())
	{
		if(styleBank.GetPositionOf(*object) == 0)
		{
			filtered.AddObject(0, object);
		}
	}

	FileOutputStream fileOutputStream(FileSystem::Path(u8"/home/amir/Desktop/_OUT/_OUT.STY"), true);
	KorgFormatWriter styleBankWriter(fileOutputStream);
	styleBankWriter.Write(filtered);*/

	return EXIT_SUCCESS;
}

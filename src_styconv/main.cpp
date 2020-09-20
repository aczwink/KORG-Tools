/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "KorgFormatReader.hpp"
#include "KorgFormatWriter.hpp"
#include "Style.hpp"

int32 Main(const String &programName, const FixedArray<String> &args)
{
	KorgFormatReader styleBankReader(fileInputStream);
	StyleBank styleBank = styleBankReader.Read();

	uint32 nPerformances = 0;
	uint32 nStyles = 0;
	stdOut << u8"Styles:" << endl;
	for(const SharedPointer<StyleBankObject>& object : styleBank.Objects())
	{
		if(object.IsInstanceOf<Style>())
		{
			const Style& style = dynamic_cast<const Style&>(*object);
			uint8 pos = styleBank.GetPositionOf(style);

			stdOut << style.Name()
				   << u8" at P" << (pos / 8)+1
				   << u8" row " << ((pos % 8) / 4)
				   << u8" col " << (pos % 4)
				   << endl;
			nStyles++;
		}
		else
			nPerformances++;
	}
	stdOut << u8"Total style count: " << nStyles << endl;
	stdOut << u8"Performances: " << nPerformances << endl;

	StyleBank filtered;
	for(const SharedPointer<StyleBankObject>& object : styleBank.Objects())
	{
		if(styleBank.GetPositionOf(*object) == 0)
		{
			filtered.AddObject(0, object);
		}
	}

	FileOutputStream fileOutputStream(FileSystem::Path(u8"/home/amir/Desktop/_OUT/_OUT.STY"), true);
	KorgFormatWriter styleBankWriter(fileOutputStream);
	styleBankWriter.Write(filtered);

	return EXIT_SUCCESS;
}

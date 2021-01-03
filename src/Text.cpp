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
//Corresponding header
#include <libkorg/Text.hpp>
//Namespaces
using namespace StdXX;

//Namespace functions
String libKORG::AccompanimentTrackNumberToAbbreviatedString(AccompanimentTrackNumber accompanimentTrackNumber)
{
	switch (accompanimentTrackNumber)
	{
		case AccompanimentTrackNumber::Drum:
			return u8"DRUM";
		case AccompanimentTrackNumber::Percussion:
			return u8"PERC";
		case AccompanimentTrackNumber::Bass:
			return u8"BASS";
		case AccompanimentTrackNumber::Accompaniment1:
			return u8"ACC1";
		case AccompanimentTrackNumber::Accompaniment2:
			return u8"ACC2";
		case AccompanimentTrackNumber::Accompaniment3:
			return u8"ACC3";
		case AccompanimentTrackNumber::Accompaniment4:
			return u8"ACC4";
		case AccompanimentTrackNumber::Accompaniment5:
			return u8"ACC5";
	}

	RAISE(ErrorHandling::IllegalCodePathError);
}

String libKORG::BankPositionToString(uint8 pos)
{
	const uint8 entriesPerColumn = 4;
	const uint8 entriesPerPage = entriesPerColumn * 2;

	const uint8 pageNumber = (pos / entriesPerPage)+1;
	const uint8 row = (pos % entriesPerPage) / entriesPerColumn;
	const uint8 col = pos % entriesPerColumn;

	return u8"P" + String::Number(pageNumber) + u8"R" + String::Number(row) + u8"C" + String::Number(col);
}

String libKORG::KeyboardTrackNumberToAbbreviatedString(KeyboardTrackNumber keyboardTrackNumber)
{
	switch (keyboardTrackNumber)
	{
		case KeyboardTrackNumber::Low:
			return u8"LOWER";
		case KeyboardTrackNumber::Upper1:
			return u8"UPPER1";
		case KeyboardTrackNumber::Upper2:
			return u8"UPPER2";
		case KeyboardTrackNumber::Upper3:
			return u8"UPPER3";
	}

	RAISE(ErrorHandling::IllegalCodePathError);
}

uint8 libKORG::ParseStyleBankFileName(const String &bankFileName)
{
	ASSERT(bankFileName.EndsWith(u8".STY"), u8"???");
	return ParseStyleBankName(bankFileName.SubString(0, bankFileName.GetLength() - 4));
}

uint8 libKORG::ParseStyleBankName(const String& string)
{
	bool isUser = string.StartsWith(u8"USER");
	bool isFavorite = string.StartsWith(u8"FAVORITE");
	ASSERT(string.StartsWith(u8"BANK") || isUser || isFavorite, u8"???");
	String bankPart = string.SubString(isFavorite ? 8 : 4);

	uint32 bankNumber = bankPart.ToUInt32() - 1;
	if(isFavorite)
		bankNumber += 20;
	else if(isUser)
		bankNumber += 17;

	return bankNumber;
}

String libKORG::StyleBankNumberToString(uint8 bankNumber)
{
	String numberAsString = String::Number(bankNumber + 1, 10, 2);
	if(bankNumber > 20)
		return u8"FAVORITE" + numberAsString;
	if(bankNumber > 17)
		return u8"USER" + numberAsString;
	return u8"BANK" + numberAsString;
}
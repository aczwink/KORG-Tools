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
#pragma once

namespace libKORG
{
	enum class AccompanimentTrackNumber
	{
		Drum = 0,
		Percussion = 1,
		Bass = 2,
		Accompaniment1 = 3,
		Accompaniment2 = 4,
		Accompaniment3 = 5,
		Accompaniment4 = 6,
		Accompaniment5 = 7
	};

	static const AccompanimentTrackNumber AccompanimentTrackNumbers[] = {
		AccompanimentTrackNumber::Drum,
		AccompanimentTrackNumber::Percussion,
		AccompanimentTrackNumber::Bass,
		AccompanimentTrackNumber::Accompaniment1,
		AccompanimentTrackNumber::Accompaniment2,
		AccompanimentTrackNumber::Accompaniment3,
		AccompanimentTrackNumber::Accompaniment4,
		AccompanimentTrackNumber::Accompaniment5
	};

	enum class KeyboardTrackNumber
	{
		Low = 0,
		Upper1 = 1,
		Upper2 = 2,
		Upper3 = 3
	};

	static const KeyboardTrackNumber KeyboardTrackNumbers[] = {
		KeyboardTrackNumber::Low,
		KeyboardTrackNumber::Upper1,
		KeyboardTrackNumber::Upper2,
		KeyboardTrackNumber::Upper3,
	};
}
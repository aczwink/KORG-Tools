/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
#include <StdXX.hpp>
#include "libkorg/BankFormat/PerformanceObject.hpp"
#include "libkorg/BankFormat/Style.hpp"

namespace libKORG
{
	StdXX::String AccompanimentTrackNumberToAbbreviatedString(AccompanimentTrackNumber accompanimentTrackNumber);
	StdXX::String BankPositionToString(uint8 bankPosition);
	StdXX::String KeyboardTrackNumberToAbbreviatedString(KeyboardTrackNumber keyboardTrackNumber);
}
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
#include <libkorg.hpp>
using namespace libKORG;

inline const PerformanceObject& ExtractFirstPerformance(const Set& set)
{
	return set.performanceBanks.Entries().begin().operator*().bank.Objects().begin().operator*().object.operator*();
}

inline const SoundObject& ExtractFirstSound(const Set& set)
{
	return set.soundBanks.Entries().begin().operator*().bank.Objects().begin().operator*().object.operator*();
}

inline const FullStyle& ExtractFirstStyle(const Set& set)
{
	return set.styleBanks.Entries().begin().operator*().bank.Objects().begin().operator*().object.operator*();
}
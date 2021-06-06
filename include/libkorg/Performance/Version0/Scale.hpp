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
#pragma once
#include <libkorg/Pitch.hpp>

namespace libKORG::Performance::V0
{
	enum class ScaleType : uint8
	{
		Equal = 0,
		//TODO: REST
		Arabic = 3,
		//TODO: REST
		User = 10,
	};

	struct ScaleSettings
	{
		ScaleType scaleType = ScaleType::Equal;
		OctavePitch key = OctavePitch::C;
		int8 pitchTune[12] = {};

		//Operators
		bool operator==(const ScaleSettings&) const = default;
	};
}
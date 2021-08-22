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
//Local
#include <libkorg/Performance/Version1/AccompanimentSettings.hpp>
#include <libkorg/Performance/Version1/GeneralTracksSettings.hpp>
#include "FX.hpp"
#include "TrackSettings.hpp"

namespace libKORG::Performance::V2
{
	struct UnknownAccSettings
	{
		StdXX::DynamicByteBuffer _0x00000008_chunk;
	};

	struct AccompanimentSettings : public libKORG::Performance::V1::GeneralTracksSettings
	{
		libKORG::Performance::V1::_0x06000008_chunk _0x06000008_data;
		StdXX::StaticArray<MasterFXSettings, 3> masterFxSettings;
		StdXX::StaticArray<StdXX::StaticArray<InsertFXSettings, 2>, 8> insertFXSettings;
		StdXX::StaticArray<TrackSettings, 8> trackSettings;
		UnknownAccSettings unknownAccSettings;
	};
}
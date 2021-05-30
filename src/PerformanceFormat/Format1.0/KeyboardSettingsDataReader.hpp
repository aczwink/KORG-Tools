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
//Local
#include <libkorg/Performance/Version1/KeyboardSettings.hpp>

class KeyboardSettingsDataReader
{
public:
	//Methods
	void Read0x0FChunk(StdXX::String& name, uint32& unknown1, StdXX::DataReader &dataReader);
	void Read0x10000008Chunk(libKORG::Performance::V1::_0x10000008_chunk& chunk, StdXX::DataReader &dataReader);
	void Read0x11000008Chunk(libKORG::Performance::V1::_0x11000008_chunk& chunk, StdXX::DataReader &dataReader);
	void Read0x12000108Chunk(libKORG::Performance::V1::_0x12000108_chunk& chunk, StdXX::DataReader &dataReader);
	void Read0x14000008Chunk(libKORG::Performance::V1::_0x14000008_chunk& chunk, StdXX::DataReader &dataReader);
	void Read0x18000008Chunk(libKORG::Performance::V1::_0x18000008_chunk& chunk, StdXX::DataReader &dataReader);
};
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
#include <libkorg/ChunkFormat.hpp>
#include <libkorg/Performance.hpp>

template<uint32 N>
class TrackPropertiesReader
{
public:
	//Constructor
	inline TrackPropertiesReader(StdXX::StaticArray<libKORG::TrackProperties, N>& trackProperties) : trackProperties(trackProperties)
	{
	}

	//Methods
	void Read(const libKORG::ChunkHeader &header, StdXX::DataReader &dataReader);

private:
	//Members
	StdXX::StaticArray<libKORG::TrackProperties, N>& trackProperties;

	//Methods
	void ReadVersion00Chunk(StdXX::DataReader& dataReader);
	void ReadVersion10Chunk(StdXX::DataReader& dataReader);
	void ReadVersion11Chunk(StdXX::DataReader& dataReader);
};
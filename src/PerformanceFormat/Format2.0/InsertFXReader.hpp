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

class InsertFXReader
{
public:
	//Constructor
	inline InsertFXReader()
	{
	}

	//Methods
	void Read(libKORG::Performance::V2::InsertFXSettings& insertFxSettings, StdXX::DataReader& dataReader)
	{
		auto& data = insertFxSettings;

		data.unknown1 = dataReader.ReadByte();
		data.unknown2 = dataReader.ReadInt16();
		dataReader.ReadBytes(data.unknown3, sizeof(data.unknown3));
		data.unknown4 = dataReader.ReadByte();
		data.unknown5 = dataReader.ReadByte();
		data.unknown6 = dataReader.ReadByte();
	}
};
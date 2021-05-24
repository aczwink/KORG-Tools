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

class PadSettingsReader
{
public:
	//Constructor
	inline PadSettingsReader(StdXX::StaticArray<libKORG::Performance::PadSettings, 4>& padSettings) : padSettings(padSettings)
	{
	}

	//Methods
	void Read(StdXX::DataReader& dataReader)
	{
		int32 index = dataReader.ReadInt32();

		auto& data = this->padSettings[index];

		data.unknown2 = dataReader.ReadUInt32();
		data.unknown3 = dataReader.ReadByte();
		data.unknown4 = dataReader.ReadInt8();
		dataReader.ReadBytes(data.unknown5, sizeof(data.unknown5));
		dataReader.ReadBytes(data.unknown6, sizeof(data.unknown6));
		data.unknown7 = dataReader.ReadByte();
	}

private:
	//Members
	StdXX::StaticArray<libKORG::Performance::PadSettings, 4>& padSettings;
};
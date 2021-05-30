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

class FXReader
{
public:
	//Methods
	template<uint32 N>
	void Read(StdXX::StaticArray<libKORG::Performance::V1::FXData, N>& fxData, StdXX::DataReader &dataReader)
	{
		int32 index = dataReader.ReadInt32();
		this->ReadData(fxData[index], dataReader);
	}

private:
	//Methods
	void ReadData(libKORG::Performance::V1::FXData& fxData, StdXX::DataReader &dataReader)
	{
		fxData.unknown2 = dataReader.ReadInt8();
		fxData.unknown3 = dataReader.ReadInt8();
		fxData.unknown4 = dataReader.ReadByte();
		fxData.unknown5 = dataReader.ReadByte();
		fxData.unknown6 = dataReader.ReadInt8();
		fxData.unknown7 = dataReader.ReadByte();
		fxData.unknown8 = dataReader.ReadByte();
		fxData.unknown9 = dataReader.ReadByte();
		fxData.unknown10 = dataReader.ReadInt16();
		dataReader.ReadBytes(fxData.unknown11, sizeof(fxData.unknown11));
	}
};
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

class StyleInfoDataReader
{
public:
	//Methods
	void Read(libKORG::Style::StyleInfoData& styleInfoData, uint8 versionMinor, StdXX::DataReader& dataReader)
	{
		auto& chunk = styleInfoData;

		uint8 nameLength = dataReader.ReadByte();
		StdXX::TextReader textReader(dataReader.InputStream(), StdXX::TextCodecType::ASCII);

		chunk.name = textReader.ReadString(nameLength);

		chunk.unknown111 = dataReader.ReadInt16();
		chunk.unknown3 = dataReader.ReadByte();

		chunk.enabledStyleElements = dataReader.ReadUInt16();

		chunk.unknown6 = dataReader.ReadByte();

		chunk.unknown7 = dataReader.ReadByte();
		chunk.unknown8 = dataReader.ReadByte();
		chunk.unknown9 = dataReader.ReadByte();
		chunk.unknown10 = dataReader.ReadByte();
		chunk.unknown11 = dataReader.ReadByte();
		chunk.unknown12 = dataReader.ReadByte();
		chunk.unknown13 = dataReader.ReadByte();
		chunk.unknown14 = dataReader.ReadByte();

		chunk.unknown15 = dataReader.ReadByte();
		chunk.unknown16 = dataReader.ReadByte();

		if(versionMinor > 2)
			chunk.unknown19 = dataReader.ReadInt16();
		else
			chunk.unknown19 = chunk.enabledStyleElements;
	}
};
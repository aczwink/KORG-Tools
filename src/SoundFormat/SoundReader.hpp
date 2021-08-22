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
//Local
#include <libkorg/BankFormat/BankObject.hpp>
#include <libkorg/ChunkFormat/ChunkFormat.hpp>
#include <libkorg/BankFormat/SoundObject.hpp>
#include <libkorg/FormatException.hpp>
#include "Format3.0/SoundFormat3_0Reader.hpp"
#include "Format2.0/SoundFormat2_0Reader.hpp"
#include "Format0.0/SoundFormat0_0Reader.hpp"
#include "LegacyFormat0.3/LegacySoundFormat0_3Reader.hpp"
#include "LegacyFormat0.0/LegacySoundFormat0_0Reader.hpp"
#include "LegacyFormat0.2/LegacySoundFormat0_2Reader.hpp"

using namespace StdXX;

class SoundReader
{
public:
	SoundObject* ReadData(const libKORG::ChunkVersion& chunkVersion, DataReader& dataReader)
	{
		switch (chunkVersion.AsUInt16())
		{
			case 0x0000:
			{
				SoundFormat0_0Reader soundFormat00Reader;
				soundFormat00Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat00Reader.data));
			}
			case 0x0100:
			{
				SoundFormat1_0Reader soundFormat10Reader;
				soundFormat10Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat10Reader.data));
			}
			case 0x0200:
			{
				SoundFormat2_0Reader soundFormat20Reader;
				soundFormat20Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat20Reader.data));
			}
			case 0x0300:
			{
				SoundFormat3_0Reader soundFormat30Reader;
				soundFormat30Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat30Reader.data));
			}
			default:
			{
				stdErr << u8"Unknown sound version: " << String::HexNumber(chunkVersion.AsUInt16(), 4) << u8" skipping..." << endl;
				NullOutputStream nullOutputStream;
				dataReader.InputStream().FlushTo(nullOutputStream);
			}
		}

		return nullptr;
	}

	SoundObject* ReadLegacyData(const libKORG::ChunkVersion& chunkVersion, DataReader& dataReader)
	{
		switch (chunkVersion.AsUInt16())
		{
			case 0x0000:
			{
				LegacySoundFormat0_0Reader soundFormat00Reader;
				soundFormat00Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat00Reader.data));
			}
			case 0x0002:
			{
				LegacySoundFormat0_2Reader soundFormat02Reader;
				soundFormat02Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat02Reader.data));
			}
			case 0x0003:
			{
				LegacySoundFormat0_3Reader soundFormat03Reader;
				soundFormat03Reader.Read(dataReader);
				return new SoundObject(Move(soundFormat03Reader.data));
			}
		}

		throw libKORG::FormatException(u8"Unknown legacy sound version: " + String::HexNumber(chunkVersion.AsUInt16(), 4));
		return nullptr;
	}
};
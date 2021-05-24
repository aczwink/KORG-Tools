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
//Class header
#include "KeyboardSettingsReader.hpp"
//Local
#include "KeyboardSettingsV1_0Reader.hpp"
#include "KeyboardSettingsV0_0Reader.hpp"

//Protected methods
libKORG::ChunkReader *KeyboardSettingsReader::OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader)
{
	switch(chunkHeader.type)
	{
		case 10:
		{
			switch (chunkHeader.version.AsUInt16())
			{
				case 0x0000:
					this->subReader = new KeyboardSettingsV0_0Reader(this->keyboardSettings, this->perfIndex);
					break;
				case 0x0100:
					this->subReader = new KeyboardSettingsV1_0Reader(this->keyboardSettings, this->perfIndex);
					break;
			}
			return this->subReader.operator->();
		}
	}

	return nullptr;
}

void KeyboardSettingsReader::ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader)
{

}

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
#include <libkorg/ChunkFormat/ChunkReader.hpp>
#include <libkorg/BankFormat/PerformanceObject.hpp>

template<typename KeyboardSettingsType, typename ReaderType, uint16 Version>
class KeyboardSettingsReader : public libKORG::ChunkReader
{
public:
	//Constructor
	inline KeyboardSettingsReader(StdXX::StaticArray<KeyboardSettingsType, 4> &keyboardSettings)
	: keyboardSettings(keyboardSettings)
	{
		this->perfIndex = 0;
	}

	//Properties
	inline void PerfIndex(uint8 newValue)
	{
		this->perfIndex = newValue;
	}

protected:
	ChunkReader *OnEnteringChunk(const libKORG::ChunkHeader &chunkHeader) override
	{
		switch(chunkHeader.type)
		{
			case 10:
			{
				switch (chunkHeader.version.AsUInt16())
				{
					case Version:
						this->subReader = new ReaderType(this->keyboardSettings, this->perfIndex);
						break;
				}
				return this->subReader.operator->();
			}
		}

		return nullptr;
	}

	void ReadDataChunk(const libKORG::ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override
	{
	}

private:
	//Members
	uint8 perfIndex;
	StdXX::StaticArray<KeyboardSettingsType, 4>& keyboardSettings;
	StdXX::UniquePointer<libKORG::ChunkReader> subReader;
};
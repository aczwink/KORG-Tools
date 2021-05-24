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
#include "../Format0.0/SoundFormat0_0Reader.hpp"

class SoundFormat1_0Reader : public SoundFormat0_0Reader
{
protected:
	//Methods
	void ReadMaxTimeAndRange(StdXX::DataReader &dataReader) override;
	void ReadOSCTrigger(libKORG::Sound::OscillatorData &oscillatorData, StdXX::DataReader &dataReader) override;

	//Inline
	inline bool ReadBool(StdXX::DataReader& dataReader)
	{
		uint8 value = dataReader.ReadByte();
		ASSERT(value <= 1, StdXX::String::Number(value));
		return value == 1;
	}
};
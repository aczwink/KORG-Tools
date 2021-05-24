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
//Local
#include "../LegacyFormat0.3/LegacySoundFormat0_3Reader.hpp"

class SoundFormat0_0Reader : public LegacySoundFormat0_3Reader
{
protected:
	//Methods
	void ReadFX(libKORG::Sound::EffectData &effectData, StdXX::DataReader &dataReader) override;
	void ReadUnknowns10(StdXX::DataReader& dataReader) override;
	void ReadUnknowns11(libKORG::Sound::OscillatorData& oscillatorData, StdXX::DataReader &dataReader) override;

	//Overrideable
	uint16 ReadMultiSampleMap(StdXX::DataReader &dataReader) override;
	void SkipUnknownBytes(StdXX::DataReader &dataReader) override;
};
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
#include "../LegacyFormat0.0/LegacySoundFormat0_0Reader.hpp"

class LegacySoundFormat0_3Reader : public LegacySoundFormat0_0Reader
{
protected:
	//Methods
	void ReadDrumKitSoundData(libKORG::Sound::DrumKitSoundData &drumKitSoundData, StdXX::DataReader &dataReader) override;
	void ReadKeyRange(libKORG::Sound::KeyRange &keyRange, StdXX::DataReader &dataReader) override;
	uint8 ReadLowPriority(StdXX::DataReader &dataReader) override;
	uint64 ReadMultiSampleId(StdXX::DataReader &dataReader) override;
	uint16 ReadMultiSampleMap(StdXX::DataReader &dataReader) override;
	void ReadScaledVelocity(libKORG::Sound::OscillatorData &oscillatorData, StdXX::DataReader &dataReader) override;
	void ReadUnknowns21(libKORG::Sound::OscillatorData &oscillatorData, StdXX::DataReader &dataReader) override;

	virtual void ReadLayerEntryDetails(libKORG::Sound::LayerEntry& layerEntry, StdXX::DataReader& dataReader);
	virtual void ReadUnknowns4(libKORG::Sound::KeyTableEntry& keyTableEntry, StdXX::DataReader& dataReader);
	virtual void SkipUnknownBytes(StdXX::DataReader& dataReader);
	virtual void SkipUnknownKeyTableEntryByte(StdXX::DataReader& dataReader);

private:
	//Methods
	void ReadKeyTableEntry(libKORG::Sound::KeyTableEntry& keyTableEntry, StdXX::DataReader& dataReader);
	void ReadLayerEntry(libKORG::Sound::LayerEntry& layerEntry, bool hasId, StdXX::DataReader& dataReader);
};
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
#include "TrackSettingsReader.hpp"
//Local
#include "../../Reading/PerformanceDataFormat.hpp"
#include "../Format2.0/InsertFXReader.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Performance;
using namespace StdXX;

//Public methods
void TrackSettingsReader::Read(libKORG::Performance::V1::TrackSettings& trackSettings, DataReader &dataReader)
{
	auto& atp = trackSettings;

	atp.unknown1 = dataReader.ReadUInt32();
	dataReader.ReadBytes(atp.unknown2, sizeof(atp.unknown2));

	atp.soundProgramChangeSeq = ProgramChangeSequence::FromUInt32(dataReader.ReadUInt32());

	atp.volume = dataReader.ReadByte();
	atp.pan = dataReader.ReadInt8() - c_knob_offset;
	atp.detune = dataReader.ReadInt8() - c_knob_offset;

	atp.octaveTranspose = dataReader.ReadInt8() / 12;

	dataReader.ReadBytes(atp.unknown3, sizeof(atp.unknown3));

	atp.fxMaster1 = dataReader.ReadByte();
	atp.fxMaster2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	atp.dry = dataReader.ReadByte();
	ASSERT(atp.dry <= 1, "??");

	atp.unknown12 = dataReader.ReadUInt32();

	atp.pbSensitivity = dataReader.ReadByte();

	atp.unknown182 = dataReader.ReadByte();
	atp.unknown181 = dataReader.ReadInt8();

	atp.lowGainTimes2 = dataReader.ReadInt8();
	atp.midGainTimes2 = dataReader.ReadInt8();
	atp.highGainTimes2 = dataReader.ReadInt8();

	dataReader.ReadBytes(atp.unknown6, sizeof(atp.unknown6));

	for(uint32 & i : atp.unknown4)
		i = dataReader.ReadUInt32();

	for(uint8 i = 0; i < 8; i++)
		dataReader.ReadBytes(atp.unknown5[i], sizeof(atp.unknown5[i]));
}
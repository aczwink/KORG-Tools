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
#include <libkorg/ChunkFormat/ChunkFormat.hpp>

namespace libKORG::BankFormat
{
	enum class ChunkType
	{
		Container = 1,
		KorgFile = 2,
		ObjectTOC = 5,
		StyleObject = 6,
		OldSoundDataMaybe = 7,
		SongBookListData = 8,
		PerformancesData = 9,
		PadData = 12,
		SoundData = 16,
		MultiSampleData = 17,
		PCMData = 18,
		CrossReferenceTable = 0xFE,
	};

	enum class ObjectType
	{
		Performance = 1,
		Style = 2,
		Sound = 4,
		MultiSample = 5,
		PCM = 6,
		StylePerformances = 7,
		PAD = 9,
		SongBookEntry = 11,
		SongBook = 12,
	};

	enum class SerialNumberType
	{
		Machine = 0,
		FMDriver = 1,
	};

	enum class EncryptionAlgorithm
	{
		Normal = 0,
		DES = 1,
		Mixed = 2,
		Blowfish = 3,
	};

	struct EncryptionInformation
	{
		SerialNumberType serialNumberType;
		EncryptionAlgorithm encryptionAlgorithm;
		uint16 vendorId;
		uint16 featureId;
	};

	struct HeaderEntry
	{
		StdXX::String name;
		ObjectType type;
		libKORG::ChunkVersion dataVersion;
		uint8 pos;
		EncryptionInformation encryptionInformation;
	};

	const uint32 HEADERENTRY_NAME_SIZE = 18;
	const uint32 OBJECTTOC_LINESIZE = 24;
}
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
#include <StdXX.hpp>
#include "libkorg/BankFormat/BankObject.hpp"
#include "libkorg/ProgramChangeSequence.hpp"
#include "libkorg/UnknownChunk.hpp"

namespace libKORG
{
	struct TrackProperties
	{
		StdXX::DynamicByteBuffer unknown1;
		ProgramChangeSequence soundProgramChangeSeq;
		uint8 volume;
		int8 pan;
		int8 detune;
		int8 octaveTranspose;
		StdXX::DynamicByteBuffer unknown2;
		uint8 fxMaster1;
		uint8 fxMaster2;
		uint8 dry;
		uint8 unknown12;
		uint8 unknown13;
		uint8 pbSensitivity;
		uint8 unknown182;
		uint8 unknown181;
		int8 lowGainTimes2;
		int8 midGainTimes2;
		int8 highGainTimes2;
		StdXX::DynamicByteBuffer unknown3;

		//Constructors
		TrackProperties() = default;
		TrackProperties(const TrackProperties& trackProperties) = default;

		//Operators
		TrackProperties& operator=(const TrackProperties& trackProperties) = default;
	};

	struct GeneralPerformanceSettings
	{
		//Constructors
		GeneralPerformanceSettings() = default;
		GeneralPerformanceSettings(const GeneralPerformanceSettings& generalPerformanceSettings) = default;
	};

	struct AccompanimentSettings : public GeneralPerformanceSettings
	{
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksBefore9;
		StdXX::StaticArray<struct TrackProperties, 8> trackProperties;
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAfter9;

		//Constructors
		AccompanimentSettings() = default;

		inline AccompanimentSettings(const AccompanimentSettings& accompanimentSettings) : GeneralPerformanceSettings(accompanimentSettings)
		{
			this->unknownChunksBefore9 = accompanimentSettings.unknownChunksBefore9;
			this->trackProperties = accompanimentSettings.trackProperties;
			this->unknownChunksAfter9 = accompanimentSettings.unknownChunksAfter9;
		}
	};

	struct KeyboardSettings : public GeneralPerformanceSettings
	{
		StdXX::String name;
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksBeforeTracks;
		StdXX::StaticArray<libKORG::TrackProperties, 4> trackProperties;
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAfterTracks;
	};

	class Performance : public BankFormat::BankObject
	{
	public:
		//Members
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtBeginning;
		struct AccompanimentSettings accompanimentSettings;
		libKORG::KeyboardSettings keyboardSettings;

		//Constructor
		inline Performance(StdXX::DynamicArray<libKORG::UnknownChunk>&& unknownChunksAtBeginning, struct AccompanimentSettings&& accompanimentSettings, libKORG::KeyboardSettings&& keyboardSettings)
				: unknownChunksAtBeginning(StdXX::Move(unknownChunksAtBeginning)),
				accompanimentSettings(StdXX::Move(accompanimentSettings)),
				keyboardSettings(StdXX::Move(keyboardSettings))
		{
		}
	};
}
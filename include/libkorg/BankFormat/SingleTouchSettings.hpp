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
#include "libkorg/BankFormat/PerformanceObject.hpp"
#include "libkorg/UnknownChunk.hpp"

namespace libKORG
{
	class SingleTouchSettings : public BankFormat::BankObject
	{
	public:
		//Members
		Performance::GeneralData generalData;
		StdXX::StaticArray<Performance::KeyboardSettings, 4> keyboardSettings;

		//Constructors
		inline SingleTouchSettings(const ChunkVersion& max9version,
								   Performance::GeneralData&& generalData,
							 		StdXX::StaticArray<Performance::KeyboardSettings, 4>&& keyboardSettings)
				: max9version(max9version),
				  generalData(StdXX::Move(generalData)),
				keyboardSettings(Move(keyboardSettings))
		{
		}

		SingleTouchSettings(const SingleTouchSettings& singleTouchSettings) = default;

		//Properties
		inline const ChunkVersion& Max9Version() const
		{
			return this->max9version;
		}

		inline ChunkVersion Version() const
		{
			return {static_cast<uint8>(this->max9version.major + 1), this->max9version.minor};
		}

	private:
		//Members
		ChunkVersion max9version;
	};
}
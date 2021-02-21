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
#include "Performance.hpp"
#include "UnknownChunk.hpp"

namespace libKORG
{
	class SingleTouchSettings : public BankObject
	{
	public:
		//Members
		ChunkVersion version;
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtBeginning;
		struct AccompanimentSettings accompanimentSettings;
		StdXX::StaticArray<libKORG::KeyboardSettings, 4> keyboardSettings;
		StdXX::DynamicArray<libKORG::UnknownChunk> unknownChunksAtEnd;

		//Constructors
		inline SingleTouchSettings(const ChunkVersion& version,
				StdXX::DynamicArray<libKORG::UnknownChunk>&& unknownChunksAtBeginning,
							 struct AccompanimentSettings&& accompanimentSettings,
							 		StdXX::StaticArray<libKORG::KeyboardSettings, 4>&& keyboardSettings,
							 		StdXX::DynamicArray<libKORG::UnknownChunk>&& unknownChunksAtEnd)
				: version(version),
				unknownChunksAtBeginning(StdXX::Move(unknownChunksAtBeginning)),
				accompanimentSettings(StdXX::Move(accompanimentSettings)),
				keyboardSettings(Move(keyboardSettings)),
				  unknownChunksAtEnd(Move(unknownChunksAtEnd))
		{
		}

		SingleTouchSettings(const SingleTouchSettings& singleTouchSettings) = default;
	};
}
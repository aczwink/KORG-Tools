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
//Local
#include <libkorg/Pitch.hpp>
#include <libkorg/ProgramChangeSequence.hpp>

namespace libKORG::Style
{
	enum class NTT_Type
	{
		Parallel = 0,
		Fixed = 1
	};

	enum class NTT_Table_Parallel
	{
		Root = 0,
		Fifth = 1,
		//TODO: rest
	};

	enum class NTT_Table_Fixed
	{
		Chord = 0,
		//TODO: rest
	};

	class NTT
	{
	public:
		//Constructor
		inline NTT(uint8 encoded) : bitfield(encoded)
		{
		}

		//Properties
		inline const StdXX::Bitfield<uint8> &Encoded() const
		{
			return this->bitfield;
		}

		inline NTT_Table_Fixed TableFixed() const
		{
			return static_cast<NTT_Table_Fixed>(this->bitfield.Get(0, 5));
		}

		inline NTT_Table_Parallel TableParallel() const
		{
			return static_cast<NTT_Table_Parallel>(this->bitfield.Get(0, 5));
		}

		inline NTT_Type Type() const
		{
			return static_cast<NTT_Type>(this->bitfield.Get(5, 3));
		}

	private:
		//Members
		StdXX::Bitfield<uint8> bitfield;
	};

	struct StyleTrackData
	{
		uint8 expression;
		ProgramChangeSequence soundProgramChangeSeq;
		Pitch keyboardRangeBottom;
		Pitch keyboardRangeTop;
		uint8 unknown1[3] = {0, 0, 0};
		NTT ntt = 0;
		uint8 unknown3 = 4;
		uint8 unknown4 = 0;
	};
}
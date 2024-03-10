/*
 * Copyright (c) 2020-2024 Amir Czwink (amir130@hotmail.de)
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
#include <StdXX.hpp>

namespace libKORG
{
	struct OC31BlockHeader
	{
		bool isBackreference;
		uint16 length;

		//only valid for backreferences:
		uint8 nExtraBytes;
		uint16 distance;
	};

	struct OC31Block : public OC31BlockHeader
	{
		StdXX::UniquePointer<StdXX::FixedSizeBuffer> rawData;
		uint8 extra[3];
	};

	uint32 OC31ComputeOptimalBlockSize(const OC31BlockHeader &block);
	StdXX::DynamicArray<OC31Block> OC31ReadAllBlocks(StdXX::InputStream& inputStream);
	void OC31ReadBlockHeader(StdXX::DataReader &dataReader, OC31BlockHeader &header);
}
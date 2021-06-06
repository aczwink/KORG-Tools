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
#include <StdXX.hpp>

namespace libKORG
{
	class OC31Compressor : public StdXX::Compressor
	{
	public:
		//Constructor
		OC31Compressor(StdXX::UniquePointer<StdXX::SeekableOutputStream>&& baseStream);

		//Methods
		void Finalize() override;
		void Flush() override;
		uint32 WriteBytes(const void *source, uint32 size) override;

	private:
		//Members
		uint8 checkValue;
		uint32 uncompressedSize;
		uint64 uncompressedSizeOffset;
		StdXX::UniquePointer<StdXX::SeekableOutputStream> baseStream;
		StdXX::SlidingDictionary dictionary;

		//Methods
		void WriteUncompressedBlock(const uint8* data, uint16 length);

		//Inline
		inline void WriteByte(uint8 byte)
		{
			this->baseStream->WriteBytes(&byte, 1);
		}
	};
}
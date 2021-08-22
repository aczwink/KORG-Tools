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
#include <StdXX.hpp>

namespace libKORG
{
	class OC31Decompressor : public StdXX::Decompressor
	{
	public:
		//Constructor
		OC31Decompressor(StdXX::InputStream& inputStream);

		//Properties
		inline uint32 UncompressedSize() const
		{
			return this->uncompressedSize;
		}

		//Methods
		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;

	private:
		//Members
		uint8 computedCheck;
		uint32 uncompressedSize;
		uint32 leftUncompressedSize;
		StdXX::SlidingDictionary dictionary;
		uint16 nBytesInBuffer;
		StdXX::DataReader dataReader;

		//Methods
		void Backreference(uint16 offset, uint16 length, uint8 lastByte);
		uint16 DecompressBackreference(uint8 flagByte);
		uint16 DecompressDirect(uint8 flagByte);
		void DecompressNextBlock();

		//Inline
		inline void FillDictionaryIfEmpty()
		{
			if ((this->nBytesInBuffer == 0) && !this->inputStream.IsAtEnd())
				this->DecompressNextBlock();
		}
	};
}
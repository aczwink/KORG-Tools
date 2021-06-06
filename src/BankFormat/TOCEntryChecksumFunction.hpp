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

//TODO: looks like CRC32
class TOCEntryChecksumFunction : public StdXX::ChecksumFunction
{
public:
	//Constructor
	inline TOCEntryChecksumFunction()
	{
		this->checksum = 0;
		this->ComputeTable();
	}

	void Finish() override
	{
	}

	uint64 GetChecksum() const override
	{
		return this->checksum;
	}

	uint8 GetChecksumSize() const override
	{
		return 4;
	}

	void Update(const void *buffer, uint32 size) override
	{
		const uint8* buf = static_cast<const uint8 *>(buffer);
		while(size--)
		{
			uint32 index = this->checksum >> 24;
			index = index xor *buf++;
			this->checksum <<= 8;
			this->checksum = this->checksum xor this->table[index];
		}
	}

private:
	//Members
	StdXX::StaticArray<uint32, 0x100> table;
	uint32 checksum;

	//Methods
	void ComputeTable()
	{
		for(uint32 i = 0; i < 0x100; i++)
		{
			uint32 sum = i << 24;
			for (uint8 j = 0; j < 8; j++)
			{
				uint32 highBitSet = sum >> 31;
				sum += sum;

				if (highBitSet)
				{
					sum = sum xor 0xEDB88320;
				}
			}

			this->table[i] = sum;
		}
	}
};
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

class StyleReader : public BankFormat::BankObjectReader
{
public:
	//Functions
	static StyleReader* CreateInstance(const ChunkVersion& chunkVersion)
	{
		ASSERT_EQUALS(0, chunkVersion.major);
		ASSERT_EQUALS(0, chunkVersion.minor);

		return new StyleReader;
	}

	BankFormat::BankObject *TakeResult() override
	{
		return this->styleFormat00V50Reader.TakeResult();
	}

protected:
	//Methods
	ChunkReader* OnEnteringChunk(const ChunkHeader &chunkHeader) override
	{
		if(chunkHeader.type == 1)
		{
			switch(chunkHeader.version.AsUInt16())
			{
				case 0x0500:
					return &this->styleFormat00V50Reader;
			}
		}

		return nullptr;
	}

	void ReadDataChunk(const ChunkHeader &chunkHeader, StdXX::DataReader &dataReader) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

private:
	//Members
	StyleFormat0_0V5_0Reader styleFormat00V50Reader;

	//Constructor
	StyleReader() = default;
};
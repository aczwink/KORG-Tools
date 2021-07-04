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
//Local
#include <libkorg/Style/KORG_MIDI.hpp>

class KORG_MIDI_EventDecoder
{
public:
	//Methods
	StdXX::DynamicArray<libKORG::Style::KORG_MIDI_Event> DecodeEvents(uint16 dataLength, StdXX::DataReader& dataReader);

private:
	//Methods
	void ReadKORG_MetaEvent(libKORG::Style::KORG_MIDI_Event& event, StdXX::DataReader& dataReader);

	//Inline
	inline uint16 ReadLength(StdXX::DataReader& dataReader)
	{
		uint8 b1 = dataReader.ReadByte();
		uint8 b2 = dataReader.ReadByte();

		return (b1 << 7) | (b2 & 0x7F);
	}
};
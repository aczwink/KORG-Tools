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
//Class header
#include "KORG_MIDI_EventDecoder.hpp"
//Namespaces
using namespace libKORG::Style;
using namespace StdXX;

//Public methods
DynamicArray<KORG_MIDI_Event> KORG_MIDI_EventDecoder::DecodeEvents(uint16 dataLength, DataReader &dataReader)
{
	DynamicArray<KORG_MIDI_Event> midiEvents;
	midiEvents.EnsureCapacity(dataLength * 2 / 3); //Events are either 1 or 2 bytes, with the exception being 0x40 and end of track marker

	bool foundEndOfMarker = false;
	while(!foundEndOfMarker && dataLength)
	{
		uint8 eventType = dataReader.ReadByte();
		dataLength--;

		if(eventType & 0x80)
		{
			uint16 totalLength = 0;
			while(eventType & 0x80)
			{
				uint8 length = eventType & ~0x80;
				totalLength = (totalLength << 7) | length;

				eventType = dataReader.ReadByte();
				dataLength--;
			}

			midiEvents.Push({KORG_MIDI_EventType::DeltaTime, totalLength});
		}

		Optional<uint8> unknownAdditional;
		if(eventType & 0x40)
		{
			eventType &= ~0x40;
			ASSERT((eventType == 0)
				   || (eventType == 1)
				   || (eventType == 3)
				   || (eventType == 6), String::HexNumber(eventType));
			dataLength--;

			unknownAdditional = dataReader.ReadByte(); //TODO: no idea what that is
		}

		switch(eventType)
		{
			case 0:
			case 1:
			case 3:
			case 12:
			case 13:
			{
				uint8 value1 = dataReader.ReadByte();

				uint8 value2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value2 & 0x80);
				value2 &= ~0x80;

				dataLength -= 2;

				midiEvents.Push({(KORG_MIDI_EventType)eventType, value1, value2, unknownAdditional});
			}
				break;
			case 5:
			{
				uint8 value1 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, value1 & 0x80);
				value1 &= ~0x80;

				dataLength--;

				midiEvents.Push({KORG_MIDI_EventType::Aftertouch, value1});
			}
				break;
			case 6:
			{
				uint8 b1 = dataReader.ReadByte();
				ASSERT_EQUALS(0, b1 & 0x80);

				uint8 b2 = dataReader.ReadByte();
				ASSERT_EQUALS(0x80, b2 & 0x80);
				b2 &= ~0x80;
				dataLength -= 2;

				uint16 combined = (b2 << 7) | b1;
				int16 value1 = -8192 + combined;
				midiEvents.Push({KORG_MIDI_EventType::Bend, static_cast<uint16>(value1), 0, unknownAdditional});
			}
				break;
			case 9:
			{
				uint32 index = midiEvents.Push({});
				auto& event = midiEvents[index];
				this->ReadKORG_MetaEvent(event, dataReader);
				dataLength -= 3; //type and size
				dataLength -= event.metaEvent.dataLength;
				foundEndOfMarker = event.metaEvent.type == KORG_MIDI_MetaEventType::EndOfTrack;
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}

	return midiEvents;
}

//Private methods
void KORG_MIDI_EventDecoder::ReadKORG_MetaEvent(KORG_MIDI_Event& event, DataReader &dataReader)
{
	event.type = KORG_MIDI_EventType::MetaEvent;

	event.metaEvent.type = static_cast<KORG_MIDI_MetaEventType>(dataReader.ReadByte());
	event.metaEvent.dataLength = this->ReadLength(dataReader);

	switch(event.metaEvent.type)
	{
		case KORG_MIDI_MetaEventType::EndOfTrack:
		case KORG_MIDI_MetaEventType::UnknownMaster:
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	ASSERT((event.metaEvent.dataLength <= sizeof(event.metaEvent.data)), String::Number(event.metaEvent.dataLength));
	dataReader.ReadBytes(event.metaEvent.data, event.metaEvent.dataLength);
}
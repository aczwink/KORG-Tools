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
#include <StdXX.hpp>

namespace libKORG
{
	enum class RangedEventType
	{
		Note
	};

	struct RangedEvent
	{
		RangedEventType type;
		uint16 position;
		uint16 value1;
		uint8 value2;
		uint16 length;
	};

	class RangedEventView
	{
	public:
		//Constructor
		inline RangedEventView(const StdXX::DynamicArray<Style::KORG_MIDI_Event> &events)
		{
			this->rangedEvents = this->ComputeRanges(events);
		}

		//Properties
		inline const StdXX::DynamicArray<RangedEvent>& Events() const
		{
			return this->rangedEvents;
		}

	private:
		//Members
		StdXX::DynamicArray<RangedEvent> rangedEvents;

		//Methods
		StdXX::DynamicArray<RangedEvent> ComputeRanges(const StdXX::DynamicArray <Style::KORG_MIDI_Event> &events)
		{
			StdXX::DynamicArray<RangedEvent> result;

			StdXX::Map <uint16, StdXX::Tuple<const Style::KORG_MIDI_Event *, uint16>> openNoteEvents;

			uint16 t = 0;
			for (const Style::KORG_MIDI_Event &event : events) {
				switch (event.type) {
					case Style::KORG_MIDI_EventType::DeltaTime:
						t += event.value1;
						break;
					case Style::KORG_MIDI_EventType::NoteOff: {
						const Style::KORG_MIDI_Event &onEvent = *openNoteEvents[event.value1].Get<0>();
						uint16 startTime = openNoteEvents[event.value1].Get<1>();
						uint16 length = t - startTime;

						ASSERT_EQUALS(64, event.value2);

						result.Push({RangedEventType::Note, startTime, event.value1, onEvent.value2, length});
					}
						break;
					case Style::KORG_MIDI_EventType::NoteOn:
						openNoteEvents[event.value1] = {&event, t};
						break;
					case Style::KORG_MIDI_EventType::EndOfTrack:
					case Style::KORG_MIDI_EventType::Unknown:
						break;
					default:
						RAISE(StdXX::ErrorHandling::IllegalCodePathError);
				}
			}

			return result;
		}
	};
}
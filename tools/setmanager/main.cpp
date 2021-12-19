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
#include "SetWindow.hpp"

int32 Main(const String &programName, const FixedArray<String> &args)
{
    EventHandling::StandardEventQueue eventQueue;

    String setPath = u8"/home/amir/Desktop/korg-resources/Pa600/pa600qt-factory-set.SET";
    Set set(setPath, *FindModel(u8"pa600qt"));

    SetController controller(set);
    SetWindow* setWindow = new SetWindow(eventQueue, controller);
    setWindow->Maximize();
    setWindow->Show();

    eventQueue.ProcessEvents();

    return EXIT_SUCCESS;
}

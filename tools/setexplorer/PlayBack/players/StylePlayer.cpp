/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "StylePlayer.hpp"

//Public methods
void StylePlayer::Start()
{
	if(!this->loadedProgram.HasValue())
		return;
	this->Stop();

	this->sequencerThread = new Thread([this](){
		MIDI::EventScheduler scheduler(this->loadedProgram.Value(), *this);
		scheduler.SetTempo(this->tempo);
		while(this->looping)
			scheduler.Schedule(this->looping);

		return EXIT_SUCCESS;
	});
	this->looping = true;
	this->sequencerThread->Start();
}

void StylePlayer::Stop()
{
	this->looping = false;
	if(!this->sequencerThread.IsNull())
		this->sequencerThread->Join();
	this->sequencerThread = nullptr;
}

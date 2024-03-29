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
#pragma once
#include <StdXX.hpp>
using namespace StdXX;
using namespace StdXX::Audio;

class SingleSamplePlayer
{
public:
	//Constructor
	inline SingleSamplePlayer(Audio::DeviceContext& deviceContext)
	{
		this->source = deviceContext.CreateSource();
	}

	//Inline
	inline void PlayBuffer(Buffer* buffer)
	{
		if(this->source->IsPlaying())
			this->source->Stop();
		this->source->DequeueProcessedBuffers();

		this->source->EnqueueBuffer(*buffer);
		this->source->Play();
	}

private:
	//State
	UniquePointer<Source> source;
};
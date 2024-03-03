/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "PlayBackFactory.hpp"
#include "../PlayBackSet.hpp"

class SamplesManager : public PlayBackFactory
{
public:
	//Constructor
	inline SamplesManager(Audio::DeviceContext& deviceContext, const PlayBackSet& playBackSet) : deviceContext(deviceContext), playBackSet(playBackSet)
	{
	}

	//Destructor
	~SamplesManager();

	//Methods
	Audio::Source *CreateSource() override;
	Audio::Buffer* LoadSample(uint64 id) override;

private:
	//State
	Audio::DeviceContext& deviceContext;
	const PlayBackSet& playBackSet;
	BinaryTreeMap<uint64, Audio::Buffer*> sampleBuffers;
};
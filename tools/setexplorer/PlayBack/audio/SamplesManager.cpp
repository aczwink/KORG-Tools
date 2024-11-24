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
//Class header
#include "SamplesManager.hpp"
//Local
#include "../loading/SampleLoader.hpp"

//Destructor
SamplesManager::~SamplesManager()
{
	for(const auto& kv : this->sampleBuffers)
	{
		delete kv.value;
	}
	this->sampleBuffers.Release();
}

//Public methods
Audio::Source *SamplesManager::CreateSource()
{
	return this->deviceContext.CreateSource();
}

Audio::Buffer* SamplesManager::LoadSample(uint64 id)
{
	if(this->sampleBuffers.Contains(id))
		return this->sampleBuffers.Get(id);

	const AbstractSample& sample = this->playBackSet.GetSample(id);
	const SampleObject* sampleObject = dynamic_cast<const SampleObject *>(&sample);

	if(sampleObject)
	{
		SampleLoader sampleLoader;
		UniquePointer<Multimedia::Frame> frame = sampleLoader.LoadSignedPCM16Bit(sampleObject->data, nullptr);
		const Multimedia::AudioBuffer* audioBuffer = frame->GetAudioBuffer();

		Audio::Buffer* buffer = this->deviceContext.CreateBuffer();
		buffer->SetData(const_cast<void *>(audioBuffer->GetPlane(0)), audioBuffer->GetPlaneSize(0), sampleObject->data.sampleRate, 1);
		this->sampleBuffers[id] = buffer;

		return buffer;
	}

	return nullptr;
}

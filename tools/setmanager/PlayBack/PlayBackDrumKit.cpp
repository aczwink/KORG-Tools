/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include "PlayBackDrumKit.hpp"

//Public methods
void PlayBackDrumKit::PlayNote(uint8 note, uint8 velocity)
{
    const Sound::KeyTableEntry& keyTableEntry = this->drumKitSoundData.keyTable[note];
    const Sound::LayerEntry& layerEntry = this->drumKitSoundData.layers[keyTableEntry.index];

    //TODO: velocity sample switchting

    if(layerEntry.sampleBankNumber == 0) //ROM samples can't be played
        return;

    const auto& ds = this->playBackSet.GetDrumSample(layerEntry.drumSampleId);
    ASSERT_EQUALS(-1, ds.sampleIndexRight);

    Audio::Buffer* buffer = this->playBackFactory.LoadSample(this->playBackSet.GetSampleEntryByIndex(ds.sampleIndexLeft).id);

    auto& source = this->keySources[note];
    if(source.IsNull())
        source = this->playBackFactory.CreateSource();

    source->EnqueueBuffer(*buffer);
    source->Play();
}

void PlayBackDrumKit::StopNote(uint8 note, uint8 velocity)
{
    auto& source = this->keySources[note];
    if(source.IsNull())
        return;

    source->Stop();
    source->DequeueProcessedBuffers();
    ASSERT_EQUALS(0, source->GetNumberOfQueuedBuffers());
}

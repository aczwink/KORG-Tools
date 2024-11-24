/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX;

class SampleLoader
{
public:
    //Methods
    UniquePointer<Multimedia::Frame> LoadSignedPCM16Bit(const Sample::SampleData& sampleData, const MultiSamples::SampleEntry* multiSamplesSampleEntry);

private:
    //Methods
    Multimedia::Frame* DecodeAudio(const DynamicByteBuffer& buffer, Multimedia::CodingFormatId sourceFormatId, const Multimedia::AudioSampleFormat& sampleFormat);
};
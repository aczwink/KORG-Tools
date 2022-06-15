/*
 * Copyright (c) 2021-2022 Amir Czwink (amir130@hotmail.de)
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
#include "SampleLoader.hpp"
//Namespaces
using namespace libKORG::Sample;
using namespace StdXX::Multimedia;

//Public methods
UniquePointer<AudioFrame> SampleLoader::LoadSignedPCM16Bit(const SampleData& sampleData, const MultiSamples::SampleEntry* multiSamplesSampleEntry)
{
    AudioSampleFormat sampleFormat(1, AudioSampleType::S16, false);

    switch(sampleData.sampleFormat)
    {
        case SampleFormat::Linear_PCM_S16BE:
            return (AudioFrame*)this->DecodeAudio(sampleData.sampleBuffer, CodingFormatId::PCM_S16BE, sampleFormat);
        case SampleFormat::Compressed:
        {
            AudioBuffer* audioBuffer = new AudioBuffer(sampleData.nSamples, sampleFormat);
            Sample::Decompress(sampleData.sampleBuffer.Data(), static_cast<int16 *>(audioBuffer->GetPlane(0)), sampleData.nSamples, multiSamplesSampleEntry->compressionCoefficients[0], multiSamplesSampleEntry->compressionCoefficients[1]);
            return new AudioFrame(audioBuffer);
        }
    }
}

//Private methods
Frame* SampleLoader::DecodeAudio(const DynamicByteBuffer& buffer, CodingFormatId sourceFormatId, const AudioSampleFormat& sampleFormat)
{
    Packet sourcePacket;
    sourcePacket.Allocate(buffer.Size());
    buffer.CopyTo(sourcePacket.GetData(), 0, sourcePacket.GetSize());

    UniquePointer<AudioStream> sourceStream = new AudioStream;
    sourceStream->sampleFormat = sampleFormat;

    UniquePointer<DecoderContext> decoderContext = FormatRegistry::Instance().GetCodingFormatById(sourceFormatId)->GetBestMatchingDecoder()->CreateContext(*sourceStream.operator->());
    decoderContext->Decode(sourcePacket);

    return decoderContext->GetNextFrame();
}
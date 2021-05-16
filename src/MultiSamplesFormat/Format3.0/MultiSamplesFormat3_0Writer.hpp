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
//Local
#include <libkorg/MultiSamples/MultiSamplesData.hpp>
#include "../MultiSamplesFormatWriter.hpp"

class MultiSamplesFormat3_0Writer : public MultiSamplesFormatWriter
{
public:
	//Constructor
	inline MultiSamplesFormat3_0Writer(StdXX::DataWriter& dataWriter) : dataWriter(dataWriter), textWriter(dataWriter.Stream(), StdXX::TextCodecType::Latin1)
	{
	}

	//Methods
	void Write(const libKORG::MultiSamples::MultiSamplesData &data) override;

private:
	//Members
	StdXX::DataWriter& dataWriter;
	StdXX::TextWriter textWriter;

	//Methods
	void Write(const libKORG::MultiSamples::DrumSampleEntry& drumSampleEntry);
	void Write(const libKORG::MultiSamples::KeyboardZone& keyboardZone);
	void Write(const libKORG::MultiSamples::MultiSampleEntry& multiSampleEntry);
	void Write(const libKORG::MultiSamples::SampleEntry& sampleEntry);
};
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
#include <libkorg.hpp>
using namespace libKORG;
using namespace libKORG::Sound;
#include "FormattedOutputter.hpp"

class SoundOutputter
{
public:
	//Constructor
	inline SoundOutputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const SoundObject& soundObject) const;

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(const DrumKitSoundData& drumKitSoundData) const;
	void Output(const OscillatorData& oscillatorData) const;
	void Output(const OSCMultiSampleSettings& oscMultiSampleSettings) const;
	void OutputKeyTable(const DrumKitSoundData& drumKitSoundData) const;
	void OutputLayers(const DrumKitSoundData& drumKitSoundData) const;
};
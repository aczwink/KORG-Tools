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
#include "FormattedOutputter.hpp"

class PerformanceOutputter
{
public:
	//Constructor
	inline PerformanceOutputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const Performance& performance);
	void Output(const SingleTouchSettings& sts);

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(const AccompanimentSettings& accompanimentSettings);
	void Output(const StdXX::StaticArray<struct TrackProperties, 4>& keyboardTrackProperties);
	void Output(const StdXX::StaticArray<struct TrackProperties, 8>& accompanimentTrackProperties);
	void Output(const GeneralPerformanceSettings::_0x07000008_chunk& data);
	void Output(const GeneralPerformanceSettings::_0x08000008_chunk& data);
	void Output(const KeyboardSettings& keyboardSettings);
	void Output(const TrackProperties& trackProperties);
};
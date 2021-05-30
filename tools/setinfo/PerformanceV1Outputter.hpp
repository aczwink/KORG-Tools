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
using namespace libKORG::Performance;
#include "FormattedOutputter.hpp"

class PerformanceV1Outputter
{
public:
	//Constructor
	inline PerformanceV1Outputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const V1::_0x06000008_chunk& chunk);
	void Output(const V1::_0x10000008_chunk& chunk);
	void Output(const V1::_0x11000008_chunk& chunk);
	void Output(const V1::_0x12000108_chunk& chunk);
	void Output(const V1::_0x14000008_chunk& chunk);
	void Output(const V1::_0x15000108_chunk& chunk);
	void Output(const V1::_0x18000008_chunk& chunk);
	void Output(const V1::_0x1A000008_chunk& chunk);
	void Output(const V1::_0x1B000008_chunk& chunk);
	void Output(const V1::PerformanceData& performance);
	void Output(const V1::STSData& sts);
	void Output(const V1::TrackSettings& trackProperties);

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(const V1::_0x04000108_chunk& chunk);
	void Output(const V1::AccompanimentSettings& accompanimentSettings);
	void Output(const V1::FXData& fxData);
	void Output(const V1::KeyboardSettings& keyboardSettings);
	void Output(const V1::PadSettings& padSettings);
};
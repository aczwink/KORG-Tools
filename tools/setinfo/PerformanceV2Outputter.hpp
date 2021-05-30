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

class PerformanceV2Outputter
{
public:
	//Constructor
	inline PerformanceV2Outputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const V2::PerformanceData& performance);
	void Output(const V2::STSData& sts);

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(const V2::_0x04020008_chunk& chunk);
	void Output(const V2::_0x20000008_chunk& chunk);
	void Output(const V2::_0x21000008_chunk& chunk);
	void Output(const V2::_0x22000008_chunk& chunk);
	void Output(const V2::AccompanimentSettings& accompanimentSettings);
	void Output(const V2::InsertFXSettings& insertFXSettings);
	void Output(const V2::KeyboardSettings& keyboardSettings);
	void Output(const V2::MasterFXSettings& masterFxSettings);
	void Output(const V2::TrackSettings& trackSettings);
	void Output(const V2::UnknownSubChunk& unknownSubChunk);
};
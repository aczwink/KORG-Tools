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

class PerformanceV0Outputter
{
public:
	//Constructor
	inline PerformanceV0Outputter(FormattedOutputter& formattedOutputter) : formattedOutputter(formattedOutputter)
	{
	}

	//Methods
	void Output(const V0::PerformanceData& performance);
	void Output(const V0::ScaleSettings& scaleSettings);

private:
	//Members
	FormattedOutputter& formattedOutputter;

	//Methods
	void Output(const V0::_0x02000108_chunk& chunk);
	void Output(const V0::FX& fx);
	void Output(const V0::FXGroup& fxGroup);
	void Output(const V0::PadSettings& padSettings);
	void Output(const V0::UnknownPart1& data);
	void Output(const V0::KeyboardSettings& data);
	void Output(const V0::UnknownPart3& data);
	void Output(const V0::UnknownPart4& data);
	void Output(const V0::UnknownPart5& data);
	void Output(const V0::TrackSettings& data);
	void Output(const V0::UnknownPart11& data);
	void Output(const V0::UnknownPart12& data);
	void Output(const V0::UnknownPart13& data);
	void Output(const V0::UnknownPart14& data);
	void Output(const V0::UnknownPart16& data);
	void Output(const V0::UnknownPart17& data);
	void Output(const V0::UnknownPart18& data);
	void Output(const V0::UnknownPart19& data);
	void Output(const V0::UnknownPart20& data);
	void Output(const V0::UnknownPart21& data);
};
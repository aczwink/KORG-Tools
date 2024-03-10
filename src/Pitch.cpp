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
//Class header
#include <libkorg/Pitch.hpp>
//Namespaces
using namespace libKORG;
using namespace StdXX;

String Pitch::ToString() const
{
	String result;
	switch(this->pitchWithinOctave)
	{
		case OctavePitch::C:
			result = u8"C";
			break;
		case OctavePitch::C_SHARP:
			result = u8"C#";
			break;
		case OctavePitch::D:
			result = u8"D";
			break;
		case OctavePitch::D_SHARP:
			result = u8"D#";
			break;
		case OctavePitch::E:
			result = u8"E";
			break;
		case OctavePitch::F:
			result = u8"F";
			break;
		case OctavePitch::F_SHARP:
			result = u8"F#";
			break;
		case OctavePitch::G:
			result = u8"G";
			break;
		case OctavePitch::G_SHARP:
			result = u8"G#";
			break;
		case OctavePitch::A:
			result = u8"A";
			break;
		case OctavePitch::A_SHARP:
			result = u8"A#";
			break;
		case OctavePitch::B:
			result = u8"B";
			break;
	}

	return result + String::Number(this->octave);
}
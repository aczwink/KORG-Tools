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
#include <StdXXTest.hpp>
#include <libkorg.hpp>
#include "../EventComparison.hpp"
//Namespaces
using namespace libKORG;
using namespace libKORG::Style;
using namespace StdXX;

TEST_SUITE(SingleDrumNoteForEachStyleElementTests)
{
	void ShouldHaveSingleNoteEvent(const DynamicArray<KORG_MIDI_Event>& events, const Pitch& expectedPitch)
	{
		RangedEventView eventRangeFinder(events);
		auto eventRanges = eventRangeFinder.Events();

		ASSERT_EQUALS(1, eventRanges.GetNumberOfElements());
		auto& event = eventRanges[0];

		ASSERT_EQUALS(RangedEventType::Note, event.type);
		ASSERT_EQUALS(0, event.position);
		ASSERT_EQUALS(expectedPitch.Encode(), event.value1);
		ASSERT_EQUALS(100, event.value2);
		ASSERT_EQUALS(192, event.length);
	}

	void AllTracksShouldBeEmpty(const IChordVariationView& chordVariationView)
	{
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Drum).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Percussion).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Bass).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Accompaniment1).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Accompaniment2).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Accompaniment3).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Accompaniment4).MIDI_Events().events);
		ShouldHaveNoEvents(chordVariationView.GetTrack(AccompanimentTrackNumber::Accompaniment5).MIDI_Events().events);
	}

	void DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(const IStyleElementView& styleElementView, const Pitch& expectedPitch)
	{
		ShouldHaveSingleNoteEvent(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Drum).MIDI_Events().events, expectedPitch);

		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Percussion).MIDI_Events().events);
		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Bass).MIDI_Events().events);
		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Accompaniment1).MIDI_Events().events);
		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Accompaniment2).MIDI_Events().events);
		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Accompaniment3).MIDI_Events().events);
		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Accompaniment4).MIDI_Events().events);
		ShouldHaveNoEvents(styleElementView.GetChordVariation(0).GetTrack(AccompanimentTrackNumber::Accompaniment5).MIDI_Events().events);

		for(uint8 i = 1; i < styleElementView.GetNumberOfChordVariations(); i++)
			AllTracksShouldBeEmpty(styleElementView.GetChordVariation(i));
	}

	void AllStyleElementsShouldBeEnabled(const StyleView& styleView)
	{
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(0));
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(1));
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(2));
		ASSERT_EQUALS(true, styleView.IsVariationEnabled(3));

		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Break));

		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Ending1));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Ending2));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Ending3));

		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Intro1));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Intro2));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Intro3));

		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill1));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill2));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill3));
		ASSERT_EQUALS(true, styleView.IsStyleElementEnabled(StyleElementNumber::Fill4));
	}

	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/styles/pa600/singledrumnoteforeachstyleelement.SET");
		Set set(setPath);
		const auto& styleEntry = set.StyleBanks().begin().operator*().value.Objects().begin().operator*().value;
		const auto& styleData = styleEntry.Get<1>()->Style().data;

		StyleView styleView(styleData);
		AllStyleElementsShouldBeEnabled(styleView);

		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetVariation(0), Pitch(OctavePitch::C, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetVariation(1), Pitch(OctavePitch::C_SHARP, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetVariation(2), Pitch(OctavePitch::D, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetVariation(3), Pitch(OctavePitch::D_SHARP, 4));

		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Intro1), Pitch(OctavePitch::E, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Intro2), Pitch(OctavePitch::F, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Intro3), Pitch(OctavePitch::F_SHARP, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Fill1), Pitch(OctavePitch::G, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Fill2), Pitch(OctavePitch::G_SHARP, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Fill3), Pitch(OctavePitch::A, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Fill4), Pitch(OctavePitch::A_SHARP, 4));

		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Break), Pitch(OctavePitch::B, 4));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Ending1), Pitch(OctavePitch::C, 5));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Ending2), Pitch(OctavePitch::C_SHARP, 5));
		DrumTrackShouldContainSingleNote_OthersShouldBeEmpty(styleView.GetStyleElement(StyleElementNumber::Ending3), Pitch(OctavePitch::D, 5));
	}
}
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
//Namespaces
using namespace libKORG;
using namespace libKORG::Sound;
using namespace StdXX;

TEST_SUITE(AllValuesSetTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/sounds/pa600/allvaluesset.SET");
		Set set(setPath);
		const auto soundEntry = set.soundBanks.Entries().begin().operator*().bank.Objects().begin().operator*();
		const auto& soundData = soundEntry.object->data;

		ASSERT_EQUALS(true, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::SingleTrigger));
		ASSERT_EQUALS(true, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::Mono));
		ASSERT_EQUALS(true, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::Legato));
		ASSERT_EQUALS(true, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::HighPriority));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::LastPriority));
		ASSERT_EQUALS(true, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::Hold));

		ASSERT_EQUALS(Pitch(OctavePitch::D_SHARP, 9), soundData.transposeRangeTopKey);
		ASSERT_EQUALS(Pitch(OctavePitch::F, -1), soundData.transposeRangeBottomKey);

		ASSERT_EQUALS(1, soundData.lowPriority);

		ASSERT_EQUALS(999, soundData.maxTime);
		ASSERT_EQUALS(127, soundData.maxRange);

		ASSERT_EQUALS(1, soundData.oscillators.GetNumberOfElements());

		ASSERT_EQUALS(560, soundData.oscillators[0].high.multiSampleNumber);
		ASSERT_EQUALS(Offset::NoAttack, soundData.oscillators[0].low.offset);

		ASSERT_EQUALS(OSCTriggerModeDelay::MS5000, soundData.oscillators[0].oscTriggerModeDelay);
		ASSERT_EQUALS(72, soundData.oscillators[0].velocityZoneBottom);
		ASSERT_EQUALS(114, soundData.oscillators[0].velocityZoneTop);
		ASSERT_EQUALS(Pitch(OctavePitch::A_SHARP, 4), soundData.oscillators[0].keyboardRangeBottomKey);
		ASSERT_EQUALS(Pitch(OctavePitch::A_SHARP, 8), soundData.oscillators[0].keyboardRangeTopKey);

		ASSERT_EQUALS(OSCTriggerMode::Random, soundData.oscillators[0].oscTriggerMode);
		ASSERT_EQUALS(true, soundData.oscillators[0].oscOffWhenSoundControllersAreOn);

		ASSERT_EQUALS(-414, soundData.oscillators[0].tune);

		ASSERT_EQUALS(33, soundData.oscillators[0].scaledVelocityBottom);
		ASSERT_EQUALS(71, soundData.oscillators[0].scaledVelocityTop);

		ASSERT_EQUALS(false, soundData.drumKitData.HasValue());
	}
}
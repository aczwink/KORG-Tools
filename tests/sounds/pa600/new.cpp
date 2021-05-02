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

TEST_SUITE(NewSoundTests)
{
	TEST_CASE(Test)
	{
		FileSystem::Path setPath(u8"testdata/sounds/pa600/new.SET");
		Set set(setPath);
		const auto& soundEntry = set.SoundBanks().begin().operator*().value.Objects().begin().operator*().value;
		const auto& soundData = soundEntry.Get<1>()->data;

		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::IsDrumKit));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::SingleTrigger));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::Mono));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::Legato));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::HighPriority));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::LastPriority));
		ASSERT_EQUALS(false, soundData.voiceAssignModeFlags.IsSet(VoiceAssignModeFlags::Hold));

		ASSERT_EQUALS(Pitch(OctavePitch::G, 9), soundData.transposeRangeTopKey);
		ASSERT_EQUALS(Pitch(OctavePitch::C, -1), soundData.transposeRangeBottomKey);

		ASSERT_EQUALS(0, soundData.lowPriority);

		ASSERT_EQUALS(15, soundData.maxTime);
		ASSERT_EQUALS(5, soundData.maxRange);

		ASSERT_EQUALS(1, soundData.oscillators.GetNumberOfElements());

		ASSERT_EQUALS(0, soundData.oscillators[0].high.multiSampleNumber);
		ASSERT_EQUALS(127, soundData.oscillators[0].high.level);
		ASSERT_EQUALS(Offset::Off, soundData.oscillators[0].high.offset);

		ASSERT_EQUALS(0, soundData.oscillators[0].low.multiSampleNumber);
		ASSERT_EQUALS(127, soundData.oscillators[0].low.level);
		ASSERT_EQUALS(false, soundData.oscillators[0].low.reversed);

		ASSERT_EQUALS(1, soundData.oscillators[0].velocityMultiSampleSwitchLowHigh);
		ASSERT_EQUALS(1, soundData.oscillators[0].velocityZoneBottom);
		ASSERT_EQUALS(127, soundData.oscillators[0].velocityZoneTop);
		ASSERT_EQUALS(Pitch(OctavePitch::C, -1), soundData.oscillators[0].keyboardRangeBottomKey);
		ASSERT_EQUALS(Pitch(OctavePitch::G, 9), soundData.oscillators[0].keyboardRangeTopKey);

		ASSERT_EQUALS(OSCTriggerMode::Normal, soundData.oscillators[0].oscTriggerMode);
		ASSERT_EQUALS(false, soundData.oscillators[0].oscOffWhenSoundControllersAreOn);

		ASSERT_EQUALS(1, soundData.oscillators[0].scaledVelocityBottom);
		ASSERT_EQUALS(127, soundData.oscillators[0].scaledVelocityTop);
	}
}
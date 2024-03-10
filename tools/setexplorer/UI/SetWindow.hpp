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
#include "BankCollectionView.hpp"
#include "PlayBackView.hpp"
#include "KeyboardView.hpp"

class SetWindow : public MainAppWindow
{
public:
    //Constructor
    SetWindow(EventHandling::EventQueue& eventQueue, SetController& setController);

private:
    //Members
    SetController& setController;

    TabContainer* tabContainer;
    BankCollectionView<PadBankNumber, StyleObject>* padBanksView;
    BankCollectionView<PerformanceBankNumber, PerformanceObject>* performanceBanksView;
    BankCollectionView<SampleBankNumber, AbstractSample>* sampleBanksView;
    BankCollectionView<SoundBankNumber, SoundObject>* soundBanksView;
    BankCollectionView<StyleBankNumber, FullStyle>* styleBanksView;
    ProgressBar* sampleSizeBar;
    PlayBackView* playBackView;
    KeyboardView* keyboardView;

    //Methods
    void CreateSampleRAMArea();
};
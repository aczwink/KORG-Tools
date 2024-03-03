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
#include "SetWindow.hpp"

//Constructor
SetWindow::SetWindow(EventHandling::EventQueue &eventQueue, SetController& setController) : MainAppWindow(eventQueue), setController(setController)
{
    const Set& set = this->setController.Set();

    this->SetTitle(set.SetPath().String() + u8" - " + set.model.GetName());

    this->padBanksView = new BankCollectionView(setController, set.padBanks);
    this->performanceBanksView = new BankCollectionView(setController, set.performanceBanks);
    this->sampleBanksView = new BankCollectionView(setController, set.sampleBanks);
    this->soundBanksView = new BankCollectionView(setController, set.soundBanks);
    this->styleBanksView = new BankCollectionView(setController, set.styleBanks);

    this->GetContentContainer()->SetLayout(new VerticalLayout);

    this->tabContainer = new TabContainer();
    this->tabContainer->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Expanding);
    this->AddContentChild(this->tabContainer);

    this->tabContainer->AddTab(u8"Pads", this->padBanksView);
    this->tabContainer->AddTab(u8"Performances", this->performanceBanksView);
    this->tabContainer->AddTab(u8"Samples", this->sampleBanksView);
    this->tabContainer->AddTab(u8"Sounds", this->soundBanksView);
    this->tabContainer->AddTab(u8"Styles", this->styleBanksView);

    this->CreateSampleRAMArea();

    this->playBackView = new PlayBackView(setController);
    this->AddContentChild(this->playBackView);

    this->keyboardView = new KeyboardView(setController);
    this->AddContentChild(this->keyboardView);
}

//Private methods
void SetWindow::CreateSampleRAMArea()
{
    const Set& set = this->setController.Set();

    uint32 usedSampleRamSize = set.ComputeUsedSampleRAMSize();
    uint32 sampleRamSize = set.model.GetSampleRAMSize() * MiB;

    GroupBox* groupBox = new GroupBox;
    groupBox->SetTitle(u8"Sample RAM");
    this->AddContentChild(groupBox);
    this->sampleSizeBar = new ProgressBar();
    groupBox->AddContentChild(this->sampleSizeBar);
    this->sampleSizeBar->Progress(usedSampleRamSize / (float64)sampleRamSize);

    CompositeWidget* row = new CompositeWidget;
    groupBox->AddContentChild(row);

    Label* label = new Label;
    label->SetText(u8"Used sample RAM: " + String::FormatBinaryPrefixed(usedSampleRamSize));
    row->AddChild(label);

    label = new Label;
    label->SetText(u8"Total sample RAM: " + String::FormatBinaryPrefixed(sampleRamSize));
    row->AddChild(label);
}

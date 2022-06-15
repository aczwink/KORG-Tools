/*
 * Copyright (c) 2020-2022 Amir Czwink (amir130@hotmail.de)
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

#include <libkorg/BankFormat/EncryptedSample.hpp>
#include <libkorg/BankFormat/Reader.hpp>
#include <libkorg/BankFormat/SampleObject.hpp>
#include <libkorg/BankFormat/Writer.hpp>

#include <libkorg/Converters/IdsCorrector.hpp>
#include <libkorg/Converters/PerformanceConverter.hpp>
#include <libkorg/Converters/PerformanceV0ToV1Converter.hpp>
#include <libkorg/Converters/PerformanceV2ToV1Converter.hpp>
#include <libkorg/Converters/SampleRemover.hpp>
#include <libkorg/Converters/SoundMapper.hpp>

#include <libkorg/DataViews/DrumKitView.hpp>
#include <libkorg/DataViews/MultiSamplesIndex.hpp>
#include <libkorg/DataViews/RangedEventView.hpp>
#include <libkorg/DataViews/SetIndex.hpp>
#include <libkorg/DataViews/StyleView.hpp>

#include <libkorg/Sample/Compression.hpp>

#include <libkorg/Model.hpp>
#include <libkorg/Package.hpp>
#include <libkorg/Set.hpp>
#include <libkorg/Text.hpp>
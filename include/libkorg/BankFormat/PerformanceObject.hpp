/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#pragma once
#include <libkorg/BankFormat/BankObject.hpp>
#include <libkorg/Performance/Version2/Data.hpp>
#include <libkorg/Performance/Version1/Data.hpp>
#include <libkorg/Performance/Version0/Data.hpp>

namespace libKORG
{
	class PerformanceObject : public BankFormat::BankObject
	{
	public:
		//Constructors
		inline PerformanceObject(StdXX::UniquePointer<Performance::V0::PerformanceData>&& v0data) : v0data(StdXX::Move(v0data))
		{
			this->version = 0;
		}

		inline PerformanceObject(StdXX::UniquePointer<Performance::V1::PerformanceData>&& v1data) : v1data(StdXX::Move(v1data))
		{
			this->version = 1;
		}

		inline PerformanceObject(StdXX::UniquePointer<Performance::V2::PerformanceData>&& v2data) : v2data(StdXX::Move(v2data))
		{
			this->version = 2;
		}

		//Properties
		inline const Performance::V0::PerformanceData& V0Data() const
		{
			ASSERT_EQUALS(0, this->version);
			return *this->v0data;
		}

		inline Performance::V1::PerformanceData& V1Data()
		{
			ASSERT_EQUALS(1, this->version);
			return *this->v1data;
		}

		inline const Performance::V1::PerformanceData& V1Data() const
		{
			ASSERT_EQUALS(1, this->version);
			return *this->v1data;
		}

		inline const Performance::V2::PerformanceData& V2Data() const
		{
			ASSERT_EQUALS(2, this->version);
			return *this->v2data;
		}

		inline uint8 Version() const
		{
			return this->version;
		}

	private:
		//Members
		uint8 version;
		StdXX::UniquePointer<Performance::V0::PerformanceData> v0data;
		StdXX::UniquePointer<Performance::V1::PerformanceData> v1data;
		StdXX::UniquePointer<Performance::V2::PerformanceData> v2data;
	};
}
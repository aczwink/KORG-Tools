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
#pragma once
#include "libkorg/BankFormat/PerformanceObject.hpp"

namespace libKORG
{
	class SingleTouchSettings : public BankFormat::BankObject
	{
	public:
		//Constructors
		inline SingleTouchSettings(StdXX::UniquePointer<Performance::V0::STSData>&& v0data) : v0data(StdXX::Move(v0data))
		{
			this->version = 1;
		}

		inline SingleTouchSettings(StdXX::UniquePointer<Performance::V1::STSData>&& v1data) : v1data(StdXX::Move(v1data))
		{
			this->version = 1;
		}

		inline SingleTouchSettings(StdXX::UniquePointer<Performance::V2::STSData>&& v2data) : v2data(StdXX::Move(v2data))
		{
			this->version = 2;
		}

		inline SingleTouchSettings(const SingleTouchSettings& sts)
		{
			this->version = sts.version;
			if(!sts.v0data.IsNull())
				this->v0data = new Performance::V0::STSData(*sts.v0data);
			if(!sts.v1data.IsNull())
				this->v1data = new Performance::V1::STSData(*sts.v1data);
			if(!sts.v2data.IsNull())
				this->v2data = new Performance::V2::STSData(*sts.v2data);
		}

		//Properties
		inline Performance::V1::STSData& V1Data()
		{
			ASSERT_EQUALS(1, this->version);
			return *this->v1data;
		}

		inline const Performance::V1::STSData& V1Data() const
		{
			ASSERT_EQUALS(1, this->version);
			return *this->v1data;
		}

		inline const Performance::V2::STSData& V2Data() const
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
		StdXX::UniquePointer<Performance::V0::STSData> v0data;
		StdXX::UniquePointer<Performance::V1::STSData> v1data;
		StdXX::UniquePointer<Performance::V2::STSData> v2data;
	};
}
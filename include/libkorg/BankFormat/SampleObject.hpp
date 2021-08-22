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
#include <StdXX.hpp>
#include <libkorg/Sample/SampleData.hpp>
#include "libkorg/BankFormat/BankObject.hpp"

namespace libKORG
{
	class SampleObject : public AbstractSample
	{
	public:
		//Members
		Sample::SampleData data;

		//Constructors
		inline SampleObject(const Sample::SampleData& data) : data(StdXX::Move(data))
		{
		}

		inline SampleObject(Sample::SampleData&& data) : data(StdXX::Move(data))
		{
		}

		//Methods
		AbstractSample *Clone() const override
		{
			return new SampleObject(this->data);
		}

		bool Equals(const AbstractSample &other) const override
		{
			const SampleObject* otherAsSampleObject = dynamic_cast<const SampleObject *>(&other);
			if(otherAsSampleObject)
				return this->data == otherAsSampleObject->data;
			return false;
		}

		uint64 GetId() const override
		{
			return this->data.id;
		}

		uint32 GetSize() const override
		{
			return this->data.sampleBuffer.Size();
		}
	};
}
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
#include "libkorg/BankFormat/BankObject.hpp"
#include "BankFormat.hpp"

namespace libKORG
{
	class EncryptedSample : public AbstractSample
	{
	public:
		//Constructor
		inline EncryptedSample(uint64 id, const ChunkVersion& dataVersion, const BankFormat::EncryptionInformation& encryptionInformation, StdXX::InputStream& inputStream)
		{
			this->id = id;
			this->dataVersion = dataVersion;
			this->encryptionInformation = encryptionInformation;
			inputStream.FlushTo(*this->buffer.CreateOutputStream());
		}

		//Properties
		inline const BankFormat::EncryptionInformation& EncryptionInfo() const
		{
			return this->encryptionInformation;
		}

		inline const ChunkVersion& DataVersion() const
		{
			return this->dataVersion;
		}

		//Methods
		uint64 GetId() const override
		{
			return this->id;
		}

		uint32 GetSize() const override
		{
			return this->buffer.Size();
		}

		//Inline
		inline StdXX::UniquePointer<StdXX::InputStream> Read() const
		{
			return this->buffer.CreateInputStream();
		}

	private:
		//Members
		uint64 id;
		ChunkVersion dataVersion;
		BankFormat::EncryptionInformation encryptionInformation;
		StdXX::DynamicByteBuffer buffer;
	};
}
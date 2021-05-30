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

namespace libKORG
{
	enum class SoundSetType
	{
		KORG = 0,
		XG = 2
	};

	class ProgramChangeSequence
	{
	public:
		//Constructors
		inline ProgramChangeSequence(bool drumKit = false)
		{
			soundSetType = SoundSetType::KORG;
			this->bankSelectMSB = 121 - (drumKit ? 1 : 0);
			this->bankSelectLSB = 0;
			this->programChange = 0;
		}

		inline ProgramChangeSequence(uint8 bankSelectMSB, uint8 bankSelectLSB, uint8 programChange)
		{
			soundSetType = SoundSetType::KORG;
			this->bankSelectMSB = bankSelectMSB;
			this->bankSelectLSB = bankSelectLSB;
			this->programChange = programChange;
		}

		inline ProgramChangeSequence(enum SoundSetType soundSetType, uint8 bankSelectMSB, uint8 bankSelectLSB, uint8 programChange)
		{
			this->soundSetType = soundSetType;
			this->bankSelectMSB = bankSelectMSB;
			this->bankSelectLSB = bankSelectLSB;
			this->programChange = programChange;
		}

		//Operators
		bool operator==(const ProgramChangeSequence&) const = default;

		inline bool operator<(const ProgramChangeSequence& rhs) const
		{
			if(this->soundSetType == rhs.soundSetType)
			{
				if(this->bankSelectMSB == rhs.bankSelectMSB)
				{
					if(this->bankSelectLSB == rhs.bankSelectLSB)
						return this->programChange < rhs.programChange;
					return this->bankSelectLSB < rhs.bankSelectLSB;
				}
				return this->bankSelectMSB < rhs.bankSelectMSB;
			}
			return this->soundSetType < rhs.soundSetType;
		}

		inline bool operator>(const ProgramChangeSequence& rhs) const
		{
			return rhs < *this;
		}

		//Properties
		inline uint8 BankSelectMSB() const
		{
			return this->bankSelectMSB;
		}

		inline uint8 BankSelectLSB() const
		{
			return this->bankSelectLSB;
		}

		inline uint8 ProgramChange() const
		{
			return this->programChange;
		}

		inline enum SoundSetType SoundSetType() const
		{
			return this->soundSetType;
		}

		//Inline
		inline void Write(StdXX::DataWriter& dataWriter) const
		{
			dataWriter.WriteByte(this->bankSelectMSB);
			dataWriter.WriteByte(this->bankSelectLSB);
			dataWriter.WriteByte(this->programChange);
		}

		inline StdXX::String ToString() const
		{
			return StdXX::String::Number(this->bankSelectMSB) + u8"." + this->ZeroFill(this->bankSelectLSB) + u8"." + this->ZeroFill(this->programChange);
		}

		//Functions
		static ProgramChangeSequence FromUInt32(uint32 encoded)
		{
			uint8 msb = encoded >> 24;
			uint8 lsb = (encoded >> 16) & 0xFF;
			uint8 soundSetType = (encoded >> 8) & 0xFF;
			uint8 pc = encoded & 0xFF;

			ASSERT(
					(soundSetType == (uint8)SoundSetType::KORG)
					|| (soundSetType == (uint8)SoundSetType::XG), StdXX::String::Number(soundSetType));
			return ProgramChangeSequence(static_cast<enum SoundSetType>(soundSetType), msb, lsb, pc);
		}

	private:
		//Members
		enum SoundSetType soundSetType;
		uint8 bankSelectMSB;
		uint8 bankSelectLSB;
		uint8 programChange;

		//Inline
		inline StdXX::String ZeroFill(uint8 value) const
		{
			StdXX::String string = StdXX::String::Number(value);
			while(string.GetLength() < 3)
				string = u8"0" + string;
			return string;
		}
	};
}
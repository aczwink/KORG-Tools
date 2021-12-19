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
//Local
#include <libkorg/Model.hpp>
#include <libkorg/FullStyle.hpp>
#include "PerformanceObject.hpp"
#include "SoundObject.hpp"
#include "ObjectBank.hpp"
#include "Reader.hpp"

namespace libKORG
{
	template<typename ObjectType>
	class ObjectBank
	{
		struct ObjectStorageEntry
		{
			StdXX::String name;
			StdXX::UniquePointer<ObjectType> object;

			bool hasInputData;
			BankFormat::HeaderEntry headerEntry;
			uint32 dataOffset;
			BankFormat::HeaderEntry stsHeaderEntry;
			uint32 stsDataOffset;
		};

		struct ConstObjectEntry
		{
			uint8 pos;

			//Constructor
			ConstObjectEntry(const ObjectBank<ObjectType>& bank, uint8 pos) : bank(bank), pos(pos)
			{
			}

			//Properties
			inline const auto& Name() const
			{
				return this->bank.GetName(this->pos);
			}

			inline const ObjectType& Object() const
			{
				return this->bank.GetObject(this->pos);
			}

		private:
			//Members
			const ObjectBank<ObjectType>& bank;
		};

		struct ObjectEntry
		{
			uint8 pos;

			//Constructor
			ObjectEntry(ObjectBank<ObjectType>& bank, uint8 pos) : bank(bank), pos(pos)
			{
			}

			//Properties
			inline const auto& Name() const
			{
				return this->bank.GetName(this->pos);
			}

			inline ObjectType& Object()
			{
				return this->bank.GetObject(this->pos);
			}

			inline const ObjectType& Object() const
			{
				return this->bank.GetObject(this->pos);
			}

		private:
			//Members
			ObjectBank<ObjectType>& bank;
		};

		struct UnchangedObjectData
		{
			ChunkHeader header;
			StdXX::DynamicByteBuffer data;
		};

	public:
		//Constructors
		inline ObjectBank(const Model &model) : model(model)
		{
			this->wasModified = false;
		}

		inline ObjectBank(const Model &model, StdXX::UniquePointer<StdXX::SeekableInputStream>&& inputStream) : model(model), inputStream(Move(inputStream))
		{
			this->ReadObjectsMetadata();
			this->wasModified = false;
		}

		inline ObjectBank(ObjectBank&& bank) : model(bank.model)
		{
			this->inputStream = StdXX::Move(bank.inputStream);
			this->objects = StdXX::Move(bank.objects);
			this->wasModified = bank.wasModified;
		}

		//Operators
		inline ObjectEntry operator[](uint8 pos)
		{
			return ObjectEntry(*this, pos);
		}

		inline ConstObjectEntry operator[](uint8 pos) const
		{
			return ConstObjectEntry(*this, pos);
		}

		inline ObjectBank& operator=(ObjectBank&& bank)
		{
			ASSERT(this->model == bank.model, u8"Can't assign banks on sets for different models")

			this->objects = StdXX::Move(bank.objects);
			this->wasModified = bank.wasModified;
			return *this;
		}

		//Properties
		inline uint8 NumberOfSlots() const
		{
			return this->GetNumberOfSlots();
		}

		inline auto Objects() const
		{
			return this->objects.Entries()
					>> StdXX::Map<const StdXX::KeyValuePair<uint8, ObjectStorageEntry> &, ConstObjectEntry>([this](const auto &kv) {
						return ConstObjectEntry(*this, kv.key);
					});
		}

		//Methods
		StdXX::Optional<uint8> FindFreeSlot(uint8 startPos = 0) const
		{
			uint8 pos = startPos;
			do {
				if (!this->HasObject(pos))
					return pos;
				pos = (pos + 1) % this->NumberOfSlots();
			} while (pos != startPos);

			return {};
		}

		//Inline
		inline ObjectType& GetObject(uint8 pos)
		{
			auto& entry = this->objects[pos];
			if(entry.object.IsNull())
				this->LoadObject(pos);

			//since the object is accessed non-const we need to assume that it gets modified
			this->wasModified = true;
			this->objects[pos].hasInputData = false; //the input object does not count anymore

			return *this->objects[pos].object;
		}

		inline const ObjectType& GetObject(uint8 pos) const
		{
			auto& entry = this->objects[pos];
			if(entry.object.IsNull())
				this->LoadObject(pos);

			return *this->objects[pos].object;
		}

		inline void RemoveObject(uint8 pos)
		{
			this->objects.Remove(pos);
			this->wasModified = true;
		}

		inline void Save(const StdXX::FileSystem::Path& bankPath)
		{
			if(!this->wasModified)
				return;

			if(!this->objects.IsEmpty())
			{
				StdXX::FileSystem::File banksDir(bankPath.GetParent());
				if(!banksDir.Exists())
					banksDir.CreateDirectory();

				this->SaveBank(bankPath);
			}
			else
			{
				StdXX::FileSystem::File bankFile(bankPath);
				if(bankFile.Exists())
					bankFile.DeleteFile();
			}

			this->inputStream = new StdXX::FileInputStream(bankPath);
			this->objects = {};
			this->ReadObjectsMetadata();
			this->wasModified = false;
		}

		inline void SetObject(const StdXX::String &name, uint8 pos, StdXX::UniquePointer<ObjectType>&& object)
		{
			this->objects[pos] = {name, StdXX::Move(object)};
			this->wasModified = true;
		}

		inline const auto& GetName(uint8 pos) const
		{
			return this->objects.Get(pos).name;
		}

		inline bool HasObject(uint8 pos) const
		{
			return this->objects.Contains(pos);
		}

	private:
		//Members
		const Model &model;
		mutable StdXX::BinaryTreeMap<uint8, ObjectStorageEntry> objects;
		mutable StdXX::UniquePointer<StdXX::SeekableInputStream> inputStream;
		bool wasModified;

		//Methods
		void CacheBinaryObject(uint32 dataOffset, UnchangedObjectData& unchangedObjectData);
		StdXX::BinaryTreeMap<StdXX::Tuple<uint8, BankFormat::ObjectType>, UnchangedObjectData> CacheBinaryObjects();

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, FullStyle>::value, void>
		LoadObject(uint8 pos) const
		{
			const auto& bankObjectEntry = this->objects.Get(pos);

			auto style = dynamic_cast<StyleObject*>(this->LoadBankObject(bankObjectEntry.headerEntry, bankObjectEntry.dataOffset));
			auto sts = dynamic_cast<SingleTouchSettings*>(this->LoadBankObject(bankObjectEntry.stsHeaderEntry, bankObjectEntry.stsDataOffset));
			this->objects[pos].object = new FullStyle(style, sts);
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< !StdXX::Type::IsSameType<T, FullStyle>::value, void>
		LoadObject(uint8 pos) const
		{
			const auto& bankObjectEntry = this->objects.Get(pos);
			this->objects[pos].object = (ObjectType*)this->LoadBankObject(bankObjectEntry.headerEntry, bankObjectEntry.dataOffset);
		}

		BankFormat::BankObject* LoadBankObject(const BankFormat::HeaderEntry& headerEntry, uint32 dataOffset) const
		{
			BankFormat::Reader bankFormatReader;
			this->inputStream->SeekTo(dataOffset);
			return bankFormatReader.ReadBankObject(headerEntry, *this->inputStream);
		}

		void ReadObjectsMetadata()
		{
			BankFormat::Reader bankFormatReader;
			bankFormatReader.ReadMetadata(*this->inputStream);
			auto entries = bankFormatReader.TakeEntries();

			for(const auto& entry : entries)
			{
				auto& input = this->objects[entry.headerEntry.pos];

				if(entry.headerEntry.type == BankFormat::ObjectType::StylePerformances)
				{
					input.stsHeaderEntry = entry.headerEntry;
					input.stsDataOffset = entry.dataOffset;
				}
				else
				{
					input.hasInputData = true;
					input.headerEntry = entry.headerEntry;
					input.dataOffset = entry.dataOffset;

					input.name = entry.headerEntry.name;
				}
			}
		}

		void SaveBank(const StdXX::FileSystem::Path& bankPath);

		//Inline
		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, AbstractSample>::value, uint8>
		GetNumberOfSlots() const
		{
			return 128; //could theoretically be 256 but apparently the bank position is signed
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, PerformanceObject>::value, uint8>
		GetNumberOfSlots() const
		{
			return 5 * 8; //5 pages and 8 performances per page
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, SoundObject>::value, uint8>
		GetNumberOfSlots() const
		{
			return 16 * 8; //16 pages and 8 sounds per page
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, FullStyle>::value, uint8>
		GetNumberOfSlots() const
		{
			return this->model.GetBankSetup().styleBanks.nStylesPerBank;
		}

		template<typename T = ObjectType>
		inline StdXX::Type::EnableIf_t< StdXX::Type::IsSameType<T, StyleObject>::value, uint8>
		GetNumberOfSlots() const
		{
			return 4 * 8; //4 pages and 8 pads per page
		}
	};
}
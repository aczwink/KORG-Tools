/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "KorgFormatWriter.hpp"
//Local
#include "KorgFormatReader.hpp"
#include "Style.hpp"

//Methods
void KorgFormatWriter::Write(StyleBank& styleBank)
{
	this->WriteHeader(styleBank.Objects().GetNumberOfElements());
	this->WriteHeaderEntries(styleBank);
	this->WriteObjects(styleBank);
	this->WriteTOC();

	//write totalSize
	uint32 size = this->outputStream.GetCurrentOffset();
	this->outputStream.SeekTo(4);
	this->dataWriter.WriteUInt32(size - 8);
}

//Private methods
void KorgFormatWriter::WriteHeader(uint32 numberOfEntries)
{
	dataWriter.WriteUInt32(0x01000114);
	dataWriter.WriteUInt32(0); //totalSize gets written later
	dataWriter.WriteUInt32(0x02000018);
	dataWriter.WriteUInt32(12);
	dataWriter.WriteUInt32(0x0B000000);
	dataWriter.WriteUInt16(0);
	dataWriter.WriteByte(0);
	fourccWriter.WriteUInt32(FOURCC(u8"KORF"));
	dataWriter.WriteByte(0);

	this->dataObjectOffsets.Push(this->outputStream.GetCurrentOffset());

	dataWriter.WriteUInt32(0x05000018);
	dataWriter.WriteUInt32(numberOfEntries * KorgFormat::OBJECTTOC_LINESIZE);
}

void KorgFormatWriter::WriteHeaderEntries(const StyleBank& styleBank)
{
	TextWriter textWriter(this->outputStream, TextCodecType::ASCII);
	for(const SharedPointer<StyleBankObject>& styleBankObject : styleBank.Objects())
	{
		const Style* style = dynamic_cast<const Style *>(styleBankObject.operator->());

		String name;
		if(style)
			name = style->Name();

		textWriter.WriteFixedLengthString(name, KorgFormat::HEADERENTRY_NAME_SIZE);
		dataWriter.WriteByte(style ? 2 : 7);
		dataWriter.WriteByte(17);
		dataWriter.WriteByte(styleBank.GetPositionOf(*styleBankObject));
		dataWriter.WriteByte(style ? 0 : 1);
		dataWriter.WriteUInt16(0);
	}
}

void KorgFormatWriter::WriteObjects(const StyleBank& styleBank)
{
	for(const SharedPointer<StyleBankObject>& styleBankObject : styleBank.Objects())
	{
		const Style* style = dynamic_cast<const Style *>(styleBankObject.operator->());

		this->dataObjectOffsets.Push(this->outputStream.GetCurrentOffset());

		if(style)
			dataWriter.WriteUInt32(0x06000030_u32);
		else
			dataWriter.WriteUInt32(0x09010030_u32);

		uint32 sizeOffset = this->outputStream.GetCurrentOffset();
		dataWriter.WriteUInt32(0);
		styleBankObject->WriteData(dataWriter);

		/*FileOutputStream fileOutputStream(FileSystem::Path(u8"/home/amir/Desktop/_OUT/pa600qt_user01/" + String::Number(styleBank.GetPositionOf(*styleBankObject)) + (style ? style->Name() : u8"")));
		DataWriter dw = DataWriter(true, fileOutputStream);
		styleBankObject->WriteData(dw);*/

		uint32 endOffset = this->outputStream.GetCurrentOffset();

		this->outputStream.SeekTo(sizeOffset);
		dataWriter.WriteUInt32(endOffset - sizeOffset - 4);
		this->outputStream.SeekTo(endOffset);
	}
}

void KorgFormatWriter::WriteTOC()
{
	dataWriter.WriteUInt32(0xFE000018);

	uint32 tocSizeOffset = this->outputStream.GetCurrentOffset();
	dataWriter.WriteUInt32(0);

	fourccWriter.WriteUInt32(FOURCC(u8"KBEG"));

	for(uint32 offset : this->dataObjectOffsets)
	{
		dataWriter.WriteUInt32(offset);
	}

	fourccWriter.WriteUInt32(FOURCC(u8"KEND"));
	dataWriter.WriteUInt32(this->dataObjectOffsets.GetNumberOfElements());

	uint32 endOffset = this->outputStream.GetCurrentOffset();
	this->outputStream.SeekTo(tocSizeOffset);
	dataWriter.WriteUInt32(endOffset - tocSizeOffset - 4);

	this->outputStream.SeekTo(endOffset);
}
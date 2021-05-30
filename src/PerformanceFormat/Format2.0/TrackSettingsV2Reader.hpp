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
//Local
#include "InsertFXReader.hpp"

class TrackSettingsV2Reader
{
public:
	//Methods
	void Read(libKORG::Performance::V2::TrackSettings &trackSettings, uint8 minorVersion, StdXX::DataReader &dataReader)
	{
		TrackSettingsReader trackPropertiesReader;
		trackPropertiesReader.Read(trackSettings, dataReader);

		switch(minorVersion)
		{
			case 0:
			case 1:
			{
				for(uint8 i = 0; i < 2; i++)
				{
					auto& data = trackSettings.insertFxSettings[i];

					InsertFXReader insertFxReader;
					insertFxReader.Read(data, dataReader);
				}
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
};
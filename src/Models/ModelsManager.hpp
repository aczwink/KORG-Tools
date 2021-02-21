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
#include <libkorg/Model.hpp>

class ModelsManager
{
public:
	//Inline
	inline const libKORG::Model* FindModel(const StdXX::String& name)
	{
		for(const auto& model : this->models)
		{
			if(model->GetName() == name)
				return model.operator->();
		}
		return nullptr;
	}

	//Functions
	inline static ModelsManager& Instance()
	{
		static ModelsManager manager;
		return manager;
	}

private:
	//Members
	StdXX::DynamicArray<StdXX::UniquePointer<libKORG::Model>> models;

	//Constructor
	inline ModelsManager()
	{
		this->RegisterModels();
	}

	//Methods
	void RegisterModels();
};
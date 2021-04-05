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
//Class header
#include "ModelsManager.hpp"
//Local
#include "pa600.hpp"
#include "pa700.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Private methods
void ModelsManager::RegisterModels()
{
	this->models.Push(new Pa600());
	this->models.Push(new Pa600QT());
	this->models.Push(new Pa700());
	this->models.Push(new Pa700OR());
}

void ModelsManager::Release()
{
	this->models.Release();
}

//Namespace functions
const Model* libKORG::FindModel(const String& name)
{
	return ModelsManager::Instance().FindModel(name);
}
#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	for (auto& it : objects)
	{
		if (it.second)
			delete it.second;
	}
}

GameObject* GameObjectManager::GetObject(const std::string& _name)
{
	auto it = objects.find(_name);

	if (it != objects.end())
	{
		return it->second;
	}

	return nullptr;
}

GameObject* GameObjectManager::CreateObject(const std::string& _name, const std::string& _prefabName)
{
	GameObject* obj = nullptr;
	std::string name;

	auto it = objects.find(_name);
	if (it != objects.end())
	{
		name = _name + std::to_string(nameInd[_name]);
		nameInd[_name]++;
	}
	else
		name = _name;
	
	obj = new GameObject(name);

	obj->prefabName = _prefabName;
	objects.insert({ name, obj });

	orderList.push_back(obj);
	
	return obj;
}

GameObject* GameObjectManager::RenameObject(const std::string& _preName, const std::string& _name)
{
	auto it = objects.find(_preName);

	if (it != objects.end())
	{
		GameObject* obj = it->second;
		obj->name = _name;
		objects.erase(it);
		objects.insert({ _name, obj });
	}

	return nullptr;
}

void GameObjectManager::RemoveObject(const std::string& _name)
{
	for (auto listIt = orderList.begin(); listIt != orderList.end(); listIt++)
	{
		if ((*listIt)->name.compare(_name) == 0)
		{
			orderList.erase(listIt);
			break;
		}
	}

	auto it = objects.find(_name);

	if (it != objects.end())
	{
		delete it->second;
		objects.erase(it);
	}
}

void GameObjectManager::RemoveAllObject()
{
	orderList.clear();

	for (auto& it : objects)
	{
		if (it.second)
			delete it.second;
	}

	objects.clear();
}

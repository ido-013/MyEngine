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

GameObject* GameObjectManager::CreateObject(const std::string& _name)
{
	auto it = objects.find(_name);

	if (it != objects.end())
	{
		return it->second;
	}

	GameObject* obj = new GameObject(_name);
	objects.insert({ _name, obj });

	return obj;
}

GameObject* GameObjectManager::RenameObject(const std::string& _preName, const std::string& _name)
{
	auto it = objects.find(_preName);

	if (it != objects.end())
	{
		GameObject* obj = it->second;
		obj->SetName(_name);
		objects.erase(it);
		objects.insert({ _name, obj });
	}

	return nullptr;
}

void GameObjectManager::RemoveObject(const std::string& _name)
{
	auto it = objects.find(_name);

	if (it != objects.end())
	{
		delete it->second;
		objects.erase(it);
	}
}

void GameObjectManager::RemoveAllObject()
{
	for (auto& it : objects)
	{
		if (it.second)
			delete it.second;
	}

	objects.clear();
}

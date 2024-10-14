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

GameObject* GameObjectManager::GetObject(std::string name)
{
	auto it = objects.find(name);

	if (it != objects.end())
	{
		return it->second;
	}

	return nullptr;
}

GameObject* GameObjectManager::CreateObject(std::string name)
{
	GameObject* go = new GameObject(name);
	objects.insert({ name, go });

	return go;
}

void GameObjectManager::RemoveObject(std::string name)
{
	auto it = objects.find(name);

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

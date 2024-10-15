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
	auto it = objects.find(name);

	if (it != objects.end())
	{
		return it->second;
	}

	GameObject* go = new GameObject(name);
	objects.insert({ name, go });

	return go;
}

GameObject* GameObjectManager::RenameObject(std::string preName, std::string name)
{
	auto it = objects.find(preName);

	if (it != objects.end())
	{
		GameObject* go = it->second;
		go->SetName(name);
		objects.erase(it);
		objects.insert({ name, go });
	}

	return nullptr;
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

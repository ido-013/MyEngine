#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	for (auto it : objects)
	{
		if (it)
			delete it;
	}
}

void GameObjectManager::AddObject(GameObject* obj)
{
	objects.push_back(obj);
}

void GameObjectManager::RemoveObject(GameObject* obj)
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		if (*it == obj)
		{
			delete *it;
			it = objects.erase(it);
			break;
		}
	}
}

void GameObjectManager::RemoveAllObject()
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		delete *it;
	}

	objects.clear();
}

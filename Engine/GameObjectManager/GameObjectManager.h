#pragma once
#include <map>
#include <string>
#include "../GameObject/GameObject.h"

class GameObjectManager
{
private:
	std::map<std::string, GameObject*> objects;

	GameObjectManager();
	~GameObjectManager();

	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator =(const GameObjectManager&) = delete;

public:
	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}

	const std::map<std::string, GameObject*>& GetAllObject() { return objects; }
	GameObject* GetObject(std::string name);
	GameObject* AddObject(std::string name);
	void RemoveObject(std::string name);
	void RemoveAllObject();
};
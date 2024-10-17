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
	GameObject* GetObject(const std::string& _name);
	GameObject* CreateObject(const std::string& _name);
	GameObject* RenameObject(const std::string& _preName, const std::string& _name);
	void RemoveObject(const std::string& _name);
	void RemoveAllObject();
};
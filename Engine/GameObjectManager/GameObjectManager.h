#pragma once
#include <map>
#include <list>
#include <string>
#include <queue>
#include "../GameObject/GameObject.h"

class GameObjectManager
{
private:
	std::map<std::string, GameObject*> objects;
	std::map<std::string, int> nameInd;
	std::list<GameObject*> orderList;
	std::queue<std::string> removeQueue;

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
	const std::list<GameObject*>& GetAllOrderObject() { return orderList; }
	GameObject* GetObject(const std::string& _name);
	GameObject* CreateObject(const std::string& _name, const std::string& _prefabName = std::string());
	GameObject* RenameObject(const std::string& _preName, const std::string& _name);

	void ReservationRemoveObject(const std::string& _name);
	void RemoveObject(const std::string& _name);
	void RemoveAllObject();

	void Update();
};
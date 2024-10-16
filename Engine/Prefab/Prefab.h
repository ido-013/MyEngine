#pragma once

#include <string>

#include "json.hpp"

#include "../GameObjectManager/GameObjectManager.h"

using json = nlohmann::ordered_json;

class Prefab
{
public:
	Prefab(std::string _name);
	~Prefab();

	static void SavePrefab(const std::string& _name, GameObject* obj);
	static GameObject* NewGameObject(const std::string& _name, const std::string& prefabName);
};
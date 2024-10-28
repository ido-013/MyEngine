#pragma once

#include <map>
#include <string>

#include "json.hpp"

#include "../GameObjectManager/GameObjectManager.h"

using json = nlohmann::ordered_json;

class Prefab
{
public:
	Prefab(std::string _name);
	~Prefab();

	static void SavePrefab(const std::string& _name, GameObject* _obj, std::map<std::string, bool>& _isSaveComp, bool _isTemp = false);
	static GameObject* NewGameObject(const std::string& _name, const std::string& _prefabName);
	static void DeletePrefab(const std::string& _name);
};
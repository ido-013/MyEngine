#include "Serializer.h"

#include <fstream>
#include <json.hpp>

#include "../GameObjectManager/GameObjectManager.h"
#include "../ComponentManager/ComponentManager.h"
#include "../RTTI/Registry.h"
#include "../Prefab/Prefab.h"

#include "../BaseComponent.h"

using json = nlohmann::ordered_json;	// Map. Orders the order the variables were declared in

void Serializer::LoadLevel(const std::string& _filename)
{
	// Open file
	std::fstream file;
	file.open("Assets/Level/" + _filename + ".lvl", std::fstream::in);

	// Check the file is valid
	if (!file.is_open())
		throw std::invalid_argument("Serializer::LoadLevel Invalid filename " + _filename + ".lvl");

	json allDataJson;
	file >> allDataJson; // the json has all the file data

	for (auto& item : allDataJson)
	{
		auto itObj = item.find("object");

		if (itObj != item.end())
		{
			GameObject* obj = nullptr;

			auto itPrefab = item.find("prefab");
			if (itPrefab != item.end())
			{
				obj = Prefab::NewGameObject(itObj.value(), itPrefab.value());
			}
			else
			{
				obj = GameObjectManager::GetInstance().CreateObject(itObj.value());
			}

			auto itComp = item.find("components");
			if (itComp != item.end())
			{
				// iterate on the json on cmp for each component, add it
				for (auto& comp : *itComp)
				{
					auto dataIt = comp.find("type");
					if (dataIt == comp.end())	// not found
						continue;

					std::string typeName = dataIt.value().dump();	// convert to string
					typeName = typeName.substr(1, typeName.size() - 2);

					// Look in the regitstry for this type and create it
					// Add the comp to the GO
					BaseRTTI* p = Registry::GetInstance().FindAndCreate(typeName, obj);
					if (p != nullptr)
						p->LoadFromJson(comp);
				}
			}
		}
	}
}

void Serializer::SaveLevel(const std::string& _filename)
{
	json allDataJson;

	for (const auto& obj : GameObjectManager::GetInstance().GetAllObject())
	{
		json objJson;
		objJson["object"] = obj.first;

		std::string prefabName = obj.second->GetPrefabName();

		if (prefabName.empty())
		{
			json components;
			for (const auto& comp : obj.second->GetAllComponent())
			{
				BaseComponent* c = comp.second;
				components.push_back(c->SaveToJson());
			}
			objJson["components"] = components;
		}
		else
		{
			objJson["prefab"] = prefabName;
		}

		allDataJson.push_back(objJson);
	}

	// Open file
	std::fstream file;
	file.open("Assets/Level/" + _filename + ".lvl", std::fstream::out);	// Open as write mode. Create it if it does not exist!

	if (!file.is_open())
		throw std::invalid_argument("Serializer::SaveLevel file write error " + _filename + ".lvl");

	file << std::setw(2) << allDataJson;	// Separates in lines and each section

	file.close();
}

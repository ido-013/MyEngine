#include "Serializer.h"

#include <fstream>
#include <json.hpp>

#include "../GameObjectManager/GameObjectManager.h"
#include "../ComponentManager/ComponentManager.h"
#include "../BaseComponent.h"
#include "../RTTI/Registry.h"

using json = nlohmann::ordered_json;	// Map. Orders the order the variables were declared in

void Serializer::LoadLevel(const std::string& filename)
{
	// Open file
	std::fstream file;
	file.open(filename, std::fstream::in);

	// Check the file is valid
	if (!file.is_open())
		throw std::invalid_argument("Serializer::LoadLevel Invalid filename " + filename);

	json allDataJson;
	file >> allDataJson; // the json has all the file data

	for (auto& item : allDataJson)
	{
		auto objIt = item.find("object");

		if (objIt != item.end())
		{
			GameObject* go = GameObjectManager::GetInstance().CreateObject(objIt.value());

			auto compIt = item.find("components");
			if (compIt == item.end())
				continue;

			// iterate on the json on cmp for each component, add it
			for (auto& comp : *compIt)
			{
				auto dataIt = comp.find("type");
				if (dataIt == comp.end())	// not found
					continue;

				std::string typeName = dataIt.value().dump();	// convert to string
				typeName = typeName.substr(1, typeName.size() - 2);

				// Look in the regitstry for this type and create it
				// Add the comp to the GO
				BaseRTTI* p = Registry::GetInstance().FindAndCreate(typeName, go);
				if (p != nullptr)
					p->LoadFromJson(comp);
			}
		}
	}
}

void Serializer::SaveLevel(const std::string& filename)
{
	json allDataJson;

	// Counter instead of name as I do not have one
	int i = 0;

	for (const auto& obj : GameObjectManager::GetInstance().GetAllObject())
	{
		json objJson;
		objJson["object"] = obj.first;

		json components;
		for (const auto& comp : obj.second->GetAllComponent())
		{
			BaseComponent* c = comp.second;
			components.push_back(c->SaveToJson());
		}
		objJson["components"] = components;

		allDataJson.push_back(objJson);
	}

	// Open file
	std::fstream file;
	file.open(filename, std::fstream::out);	// Open as write mode. Create it if it does not exist!

	if (!file.is_open())
		throw std::invalid_argument("Serializer::SaveLevel file write error " + filename);

	file << std::setw(2) << allDataJson;	// Separates in lines and each section

	file.close();
}

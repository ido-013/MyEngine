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

	json allData;
	file >> allData;	// the json has all the file data

	for (auto& item : allData)
	{
		GameObject* go = new GameObject;
		auto objIt = item.find("object");

		if (objIt != item.end())
		{
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
				std::cout << typeName << std::endl;

				// Look in the regitstry for this type and create it
				BaseRTTI* p = Registry::GetInstance().FindAndCreate(typeName, go);
				if (p != nullptr)
					p->LoadFromJson(comp);

				// Add the comp to the GO
			}
		}
	}
}

void Serializer::SaveLevel(const std::string& filename)
{
	json allData;

	// Counter instead of name as I do not have one
	int i = 0;

	for (GameObject* go : GameObjectManager::GetInstance().GetAllObjects())
	{
		json obj;
		obj["object"] = i++;

		json components;
		for (auto comp : go->GetAllComponent())
		{
			BaseComponent* c = comp.second;
			components.push_back(c->SaveToJson());
		}
		obj["components"] = components;

		allData.push_back(obj);
	}

	// Open file
	std::fstream file;
	file.open(filename, std::fstream::out);	// Open as write mode. Create it if it does not exist!

	if (!file.is_open())
		throw std::invalid_argument("Serializer::SaveLevel file write error " + filename);

	file << std::setw(2) << allData;	// Separates in lines and each section

	file.close();
}

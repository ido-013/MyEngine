#include "Prefab.h"

#include <fstream>

#include "../Imgui/imgui_inc.h"
#include "../Editor/Util.h"

#include "../BaseComponent.h"
#include "../RTTI/Registry.h"
#include "../ResourceManager/ResourceManager.h"

Prefab::Prefab(std::string _name)
{
}

Prefab::~Prefab()
{
}

void Prefab::SavePrefab(const std::string& _name, GameObject* _obj, std::map<std::string, bool>& _isSaveComp, bool _isTemp)
{
	std::string filename = "./Assets/Prefab/" + _name + ".prefab";

	json prefab;

	json components;
	for (auto comp : _obj->GetAllComponent())
	{
		if (!_isSaveComp[comp.first] && !_isTemp)
			continue;

		BaseComponent* c = comp.second;
		components.push_back(c->SaveToJson());
	}
	prefab["components"] = components;

	std::fstream file;
	file.open(filename, std::fstream::out);

	if (!file.is_open())
		throw std::invalid_argument("Prefab::SavePrefab file write error " + filename);

	file << std::setw(2) << prefab;

	file.close();
}

GameObject* Prefab::NewGameObject(const std::string& _name, const std::string& _prefabName)
{
	GameObject* obj = GameObjectManager::GetInstance().CreateObject(_name, _prefabName);

	json* data = ResourceManager::GetInstance().GetResourcePointer<json>(_prefabName);

	auto compIt = data->find("components");
	if (compIt == data->end())
		return nullptr;

	for (auto& comp : *compIt)
	{
		auto dataIt = comp.find("type");
		if (dataIt == comp.end())
			continue;

		std::string typeName = dataIt.value().dump();
		typeName = typeName.substr(1, typeName.size() - 2);

		BaseRTTI* p = Registry::GetInstance().FindAndCreate(typeName, obj);
		if (p != nullptr)
			p->LoadFromJson(comp);
	}

	return obj;
}

void Prefab::DeletePrefab(const std::string& _name)
{
	std::filesystem::remove("Assets/Prefab/" + _name + ".prefab");
}
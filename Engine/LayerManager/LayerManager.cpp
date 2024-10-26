#include "LayerManager.h"

#include <json.hpp>
#include <fstream>

using json = nlohmann::ordered_json;

LayerManager::LayerManager()
{
	/*layers.insert({ "Default", 1 });
	layers.insert({ "UI", 0 });*/

	LoadLayer();
}

LayerManager::~LayerManager()
{
	SaveLayer();
}

const float& LayerManager::GetDepth(const std::string& _name)
{
	auto it = layers.find(_name);
	
	if (it != layers.end())
	{
		return it->second;
	}

	return layers["Default"];
}

void LayerManager::AddLayer(const std::string& _name, const float& _value)
{
	layers.insert({ _name, _value });
}

void LayerManager::DeleteLayer(const std::string& _name)
{
	if (!_name.compare("Default") || !_name.compare("UI"))
		return;

	auto it = layers.find(_name);

	if (it != layers.end())
	{
		layers.erase(it);
	}
}

void LayerManager::SaveLayer()
{
	json allDataJson;

	for (const auto& layer : layers)
	{
		json layerJson;
		layerJson["name"] = layer.first;
		layerJson["depth"] = layer.second;

		allDataJson.push_back(layerJson);
	}

	// Open file
	std::fstream file;
	file.open("Assets/Layer/layer", std::fstream::out);

	if (!file.is_open())
		throw std::invalid_argument("Serializer::SaveLevel file write error layer");

	file << std::setw(2) << allDataJson;	// Separates in lines and each section

	file.close();
}

void LayerManager::LoadLayer()
{
	// Open file
	std::fstream file;
	file.open("Assets/Layer/layer", std::fstream::in);

	// Check the file is valid
	if (!file.is_open())
		throw std::invalid_argument("Serializer::LoadLevel Invalid filename");

	json allDataJson;
	file >> allDataJson; // the json has all the file data

	for (auto& item : allDataJson)
	{
		std::string name = item.find("name").value();
		float depth = item.find("depth").value();

		AddLayer(name, depth);
	}
}

#include "LayerManager.h"

#include <json.hpp>
#include <fstream>

using json = nlohmann::ordered_json;

LayerManager::LayerManager() : layers({std::string()})
{
	AddLayer(0, "UI");
	AddLayer(maxLayerInd, "Default");

	LoadLayer();
}

LayerManager::~LayerManager()
{
	SaveLayer();
}

const std::string& LayerManager::GetName(const int& _ind)
{
	return layers[_ind];
}

const int& LayerManager::GetLayerInd(const std::string& _name)
{
	auto it = layerInds.find(_name);

	if (it == layerInds.end())
		return -1;

	return layerInds[_name];
}

void LayerManager::AddLayer(const int& _ind, const std::string& _name)
{
	layers[_ind] = _name;
	layerInds[_name] = _ind;
}

void LayerManager::DeleteLayer(const int& _ind)
{
	auto it = layerInds.find(layers[_ind]);

	if (it != layerInds.end())
	{
		layerInds.erase(it);
	}

	layers[_ind] = std::string();
}

void LayerManager::SaveLayer()
{
	json allDataJson;

	for (int i = 1; i < maxLayerInd; i++)
	{
		allDataJson[i] = layers[i];
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

	for (int i = 1; i < maxLayerInd; i++)
	{
		AddLayer(i, allDataJson[i]);
	}
}

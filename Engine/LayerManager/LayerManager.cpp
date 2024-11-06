#include "LayerManager.h"

#include <json.hpp>
#include <fstream>

using json = nlohmann::ordered_json;

LayerManager::LayerManager() : layers({std::string()})
{
	layers[0] = "UI";
	layers[maxLayerInd] = "Default";

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

void LayerManager::AddLayer(const int& _ind, const std::string& _name)
{
	layers[_ind] = _name;
}

void LayerManager::DeleteLayer(const int& _ind)
{
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
		layers[i] = allDataJson[i];
	}
}

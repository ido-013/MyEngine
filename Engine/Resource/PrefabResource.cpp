#include "PrefabResource.h"

#include <fstream>

#include "json.hpp"

using json = nlohmann::ordered_json;

PrefabResource::~PrefabResource()
{
	UnloadData();
}

void PrefabResource::LoadData(const std::string& _filename)
{
	std::fstream file;
	file.open("./Assets/Prefab/" + _filename, std::fstream::in);

	json* pJson = new json;
	file >> *pJson;

	data = pJson;
}

void PrefabResource::ReloadData(const std::string& _filename)
{
	UnloadData();
	LoadData(_filename);
}

void PrefabResource::UnloadData()
{
	if (data)
	{
		delete static_cast<json*>(data);
	}

	data = nullptr;
}

#include "PrefabResource.h"

#include <fstream>

#include "json.hpp"

using json = nlohmann::ordered_json;

PrefabResource::~PrefabResource()
{
	if (data)
	{
		delete static_cast<json*>(data);
	}

	data = nullptr;
}

void PrefabResource::LoadData(const std::string& filename)
{
	std::fstream file;
	file.open("./Assets/Prefab/" + filename, std::fstream::in);

	json* pJson = new json;
	file >> *pJson;

	data = pJson;
}
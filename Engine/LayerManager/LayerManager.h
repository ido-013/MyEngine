#pragma once

#include <map>
#include <string>

class LayerManager
{
private:	
	LayerManager();
	~LayerManager();

	LayerManager(const LayerManager&) = delete;
	LayerManager& operator =(const LayerManager&) = delete;

	std::map<std::string, float> layers;

public:
	static LayerManager& GetInstance()
	{
		static LayerManager instance;
		return instance;
	}

	const float& GetDepth(const std::string& _name);

	void AddLayer(const std::string& _name, const float& _value);
	void DeleteLayer(const std::string& _name);

	const std::map<std::string, float> GetAllLayer() { return layers; }

	void SaveLayer();
	void LoadLayer();
};
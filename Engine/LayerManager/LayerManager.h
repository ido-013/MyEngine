#pragma once

#include <array>
#include <string>

class LayerManager
{
public:
	static constexpr int maxLayerInd = 15;

private:	
	LayerManager();
	~LayerManager();

	LayerManager(const LayerManager&) = delete;
	LayerManager& operator =(const LayerManager&) = delete;

	std::array<std::string, maxLayerInd + 1> layers;

public:
	static LayerManager& GetInstance()
	{
		static LayerManager instance;
		return instance;
	}

	const std::string& GetName(const int& _ind);

	void AddLayer(const int& _ind, const std::string& _name);
	void DeleteLayer(const int& _ind);

	void SaveLayer();
	void LoadLayer();
};
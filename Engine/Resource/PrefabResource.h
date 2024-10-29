#pragma once
#include "Resource.h"

class PrefabResource : public Resource
{
public:
	PrefabResource() = default;
	~PrefabResource();

	void LoadData(const std::string& _filename) override;
	void ReloadData(const std::string& _filename) override;
	void UnloadData() override;
};
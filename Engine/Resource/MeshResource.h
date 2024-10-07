#pragma once
#include "Resource.h"

class MeshResource : public Resource
{
public:
	~MeshResource();
	void LoadData(const std::string& filename) override;
};
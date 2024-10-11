#pragma once
#include "Resource.h"

class ShaderResource : public Resource
{
public:
	~ShaderResource();
	void LoadData(const std::string& filename) override;
};
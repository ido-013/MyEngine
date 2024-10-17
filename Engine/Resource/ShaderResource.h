#pragma once
#include "Resource.h"

class ShaderResource : public Resource
{
public:
	ShaderResource() = default;
	~ShaderResource();
	void LoadData(const std::string& _filename) override;
};
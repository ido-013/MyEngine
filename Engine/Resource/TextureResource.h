#pragma once
#include "Resource.h"

class TextureResource : public Resource
{
public:
	TextureResource() = default;
	~TextureResource();
	void LoadData(const std::string& _filename) override;
};
#pragma once
#include "Resource.h"

class AudioResource : public Resource
{
public:
	AudioResource() = default;
	~AudioResource();

	void LoadData(const std::string& _filename) override;
};
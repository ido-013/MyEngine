#pragma once
#include "Resource.h"

class AudioResource : public Resource
{
private:

public:
	AudioResource() = default;
	~AudioResource();

	void LoadData(const std::string& filename) override;
};
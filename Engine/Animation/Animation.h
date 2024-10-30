#pragma once
#include <vector>
#include <string>

struct Animation
{
	std::vector<std::pair<float, std::string>> frame;
	float totalTime = 0;
	int size = 0;

	void AddFrame(const float& _time, const std::string& _spriteName);
};

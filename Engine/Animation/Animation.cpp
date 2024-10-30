#include "Animation.h"

void Animation::AddFrame(const float& _time, const std::string& _spriteName)
{
	totalTime += _time;
	size++;
	frame.push_back({ _time, _spriteName });
}

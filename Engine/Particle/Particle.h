#pragma once

#include <glm/glm.hpp>

class Particle
{
private:
	float size;
	float duration;
	int count;
	float color[4];

	float randFloat();

public:
	Particle(float _size, float _duration, int _count, float _color[4]);

	void MakeGameObject(const glm::vec2& _pos, const glm::vec2& _vel);
	void Explosion(const glm::vec2& _pos, const glm::vec2& _minVel, const glm::vec2& _velRange);
};
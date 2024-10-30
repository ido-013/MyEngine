#include "Particle.h"

#include "../GameObjectManager/GameObjectManager.h"

#include "../Components.h"

float Particle::randFloat()
{
	return rand() / static_cast<float>(RAND_MAX);
}

Particle::Particle(float _size, float _duration, int _count, float _color[4]) :
	size(_size), duration(_duration), count(_count)
{
	for (int i = 0; i < 4; i++)
	{
		color[i] = _color[i];
	}
}

void Particle::MakeGameObject(const glm::vec2& _pos, const glm::vec2& _vel)
{
	GameObject* obj = GameObjectManager::GetInstance().CreateObject("particle");

	TransformComp* t = obj->AddComponent<TransformComp>();
	SpriteComp* s = obj->AddComponent<SpriteComp>();
	RigidbodyComp* r = obj->AddComponent<RigidbodyComp>();

	t->SetPos({ _pos.x, _pos.y });
	t->SetScale({ size, size });

	s->SetColor(color[0], color[1], color[2]);
	s->SetAlpha(color[3]);

	r->AddVelocity(_vel.x, _vel.y);
}

void Particle::Explosion(const glm::vec2& _pos, const glm::vec2& _minVel, const glm::vec2& _velRange)
{
	for (int i = 0; i < count; i++)
	{
		MakeGameObject(_pos, _minVel + (_velRange * randFloat()));
	}
}
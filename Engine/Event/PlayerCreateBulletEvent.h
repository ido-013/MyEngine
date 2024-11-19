#pragma once

#include "Event.h"

struct PlayerCreateBulletEvent : public Event
{
	GameObject* bullet;

	PlayerCreateBulletEvent(GameObject* _src, GameObject* _dst, GameObject* _bullet) : Event(_src, _dst), bullet(_bullet) {}
};
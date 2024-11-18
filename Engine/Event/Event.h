#pragma once

#include <string>

#include "../GameObject/GameObject.h"

struct Event
{
	Event(GameObject* _src, GameObject* _des);
	virtual ~Event() = default;

	GameObject* src;
	GameObject* dst;
};
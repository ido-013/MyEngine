#pragma once

#include "Event.h"

struct PlayerCreateBombEvent : public Event
{
	GameObject* bomb;

	PlayerCreateBombEvent(GameObject* _src, GameObject* _dst, GameObject* _bomb) : Event(_src, _dst), bomb(_bomb) {}
};
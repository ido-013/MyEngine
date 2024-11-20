#pragma once

#include "Event.h"

struct PlayerMoveEvent : public Event
{
	PlayerMoveEvent(GameObject* _src, GameObject* _dst) : Event(_src, _dst) {}
};
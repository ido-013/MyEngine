#pragma once

#include "Event.h"

struct PlayerDeathEvent : public Event
{
	PlayerDeathEvent(GameObject* _src, GameObject* _dst) : Event(_src, _dst) {}
};
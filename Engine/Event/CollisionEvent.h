#pragma once
#include "Event.h"

struct CollisionEvent : public Event
{
	bool enter;
	bool exit;

	CollisionEvent(Entity* _src, Entity* _dst, bool _enter = false, bool _exit = false) : Event(_src, _dst), enter(_enter), exit(_exit) {}
};
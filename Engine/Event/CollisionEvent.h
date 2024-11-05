#pragma once
#include "Event.h"

struct CollisionEvent : public Event
{
	bool isPass;
	bool enter;

	CollisionEvent(Entity* _src, Entity* _dst, bool _isPass, bool _enter = false) : Event(_src, _dst), isPass(_isPass), enter(_enter) {}
};
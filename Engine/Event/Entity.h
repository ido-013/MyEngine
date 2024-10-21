#pragma once
#include "Event.h"

class Entity
{
public:
	Entity();
	~Entity();

	virtual void OnEvent(Event* _event) = 0;
};
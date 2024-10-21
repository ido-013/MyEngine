#include "Entity.h"

#include "../EventManager/EventManager.h"

Entity::Entity()
{
	EventManager::GetInstance().AddEntity(this);
}

Entity::~Entity()
{
	EventManager::GetInstance().DeleteEntity(this);
}

#include "CollisionManager.h"

#include "CollisionUtil.h"
#include "../EventManager/EventManager.h"
#include "../Event/CollisionEvent.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::AddCollider(ColliderComp* _col)
{
	colliders.push_back(_col);
}

void CollisionManager::DeleteCollider(ColliderComp* _col)
{
	for (auto it = colliders.begin(); it != colliders.end(); it++)
	{
		if (*it == _col)
		{
			colliders.erase(it);
			break;
		}
	}
}

void CollisionManager::Update()
{
	if (colliders.empty())
		return;

	EventManager& em = EventManager::GetInstance();

	for (int i = 0; i < colliders.size() - 1; i++)
	{
		for (int j = i + 1; j < colliders.size(); j++)
		{
			ColliderComp* a = colliders[i];
			ColliderComp* b = colliders[j];

			if (IsCollisionAABB(a->GetPos(), a->GetScale(), b->GetPos(), b->GetScale()))
			{
				em.AddEvent<CollisionEvent>(a, b);
				em.AddEvent<CollisionEvent>(b, a);
			}
		}
	}
}

#include "CollisionManager.h"

#include "CollisionUtil.h"

#include "../EventManager/EventManager.h"
#include "../Profiler/Profiler.h"

#include "../Event/CollisionEvent.h"


CollisionManager::CollisionManager() : flag(false)
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
	std::string s = "CollisionManager " + std::string(__FUNCTION_NAME__);
	DEBUG_PROFILER_START(s);

	if (colliders.empty())
	{
		DEBUG_PROFILER_END;
		return;
	}
	
	EventManager& em = EventManager::GetInstance();

	flag = !flag;

	for (int i = 0; i < colliders.size() - 1; i++)
	{
		for (int j = i + 1; j < colliders.size(); j++)
		{
			ColliderComp* a = colliders[i];
			ColliderComp* b = colliders[j];
			GameObject* objA = a->GetOwner();
			GameObject* objB = b->GetOwner();

			if (IsCollisionAABB(a->GetPos(), a->GetScale(), b->GetPos(), b->GetScale()))
			{
				bool isPass = false;

				if (!objA->GetLayerName().compare("Explosion") || !objB->GetLayerName().compare("Explosion"))
				{
					isPass = true;
				}

				if (currentCollisionMap.find({ a, b }) == currentCollisionMap.end())
				{
					em.AddEvent(new CollisionEvent(objA, objB, true));
					em.AddEvent(new CollisionEvent(objB, objA, true));
				}
				else
				{
					em.AddEvent(new CollisionEvent(objA, objB));
					em.AddEvent(new CollisionEvent(objB, objA));
				}

				currentCollisionMap[{ a, b }] = flag;
			}
		}
	}

	for (auto it = currentCollisionMap.begin(); it != currentCollisionMap.end();)
	{
		if (it->second != flag)
		{
			ColliderComp* a = it->first.first;
			ColliderComp* b = it->first.second;

			bool findColliderA = false;
			bool findColliderB = false;

			for (auto colIt = colliders.begin(); colIt != colliders.end(); colIt++)
			{
				if (*colIt == a)
				{
					findColliderA = true;
				}

				if (*colIt == b)
				{
					findColliderB = true;
				}
			}

			if (findColliderA && findColliderB)
			{
				em.AddEvent(new CollisionEvent(a->GetOwner(), b->GetOwner(), false, true));
				em.AddEvent(new CollisionEvent(b->GetOwner(), a->GetOwner(), false, true));
			}

			currentCollisionMap.erase(it++);
		}
		else
		{
			it++;
		}
	}

	DEBUG_PROFILER_END;
}

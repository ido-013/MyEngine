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

			if (IsCollisionAABB(a->GetPos(), a->GetScale(), b->GetPos(), b->GetScale()))
			{
				bool isPass = false;

				if (!a->GetOwner()->GetLayerName().compare("Explosion") || !b->GetOwner()->GetLayerName().compare("Explosion"))
				{
					isPass = true;
				}

				if (currentCollisionMap.find({ a, b }) == currentCollisionMap.end())
				{
					em.AddEvent(new CollisionEvent(a, b, isPass, true));
					em.AddEvent(new CollisionEvent(b, a, isPass, true));
				}
				else
				{
					em.AddEvent(new CollisionEvent(a, b, isPass));
					em.AddEvent(new CollisionEvent(b, a, isPass));
				}

				currentCollisionMap[{a, b}] = flag;
			}
		}
	}

	for (auto it = currentCollisionMap.begin(); it != currentCollisionMap.end();)
	{
		if (it->second != flag)
		{
			currentCollisionMap.erase(it++);
		}
		else
		{
			it++;
		}
	}

	DEBUG_PROFILER_END;
}

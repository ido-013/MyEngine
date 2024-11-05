#pragma once

#include <vector>
#include <map>

#include "../EngineComponent/ColliderComp.h"

class CollisionManager
{
private:
	CollisionManager();
	CollisionManager(const CollisionManager&) = delete;
	const CollisionManager& operator=(const CollisionManager&) = delete;
	~CollisionManager();

	std::vector<ColliderComp*> colliders;
	std::map<std::pair<ColliderComp*, ColliderComp*>, bool> currentCollisionMap;

	bool flag;

public:
	static CollisionManager& GetInstance()
	{
		static CollisionManager instance;
		return instance;
	}

	void AddCollider(ColliderComp* _col);
	void DeleteCollider(ColliderComp* _col);

	void Update();
};
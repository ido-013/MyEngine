#pragma once

#include "../LayerManager/LayerManager.h"

#include "Events.h"

class EventHandler
{
private:
	EventHandler();

	EventHandler(const EventHandler&) = delete;
	const EventHandler& operator=(const EventHandler) = delete;

	~EventHandler();

	static void CollisionNull(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent);

	static void CollisionWall(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent);
	static void CollisionWallBullet(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent);
	static void CollisionExplosion(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent);
	static void CollisionPlayerBullet(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent);
	static void CollisionBulletPlayer(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent);

	void (*collisionEventHandler[LayerManager::maxLayerInd + 1][LayerManager::maxLayerInd + 1])(GameObject*, GameObject*, CollisionEvent*);
	
	void SetCollsionEventHandler(void(*_func)(GameObject*, GameObject*, CollisionEvent*), const std::string& _srcLayerName = std::string(), const std::string& _dstLayerName = std::string());

public:
	static EventHandler& GetInstance()
	{
		static EventHandler instance;
		return instance;
	}

	void OnEvent(Event* _event);
};
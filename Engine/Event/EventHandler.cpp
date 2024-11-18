#include "EventHandler.h"

#include "../Components.h"

EventHandler::EventHandler()
{
	SetCollsionEventHandler(CollisionNull);

	SetCollsionEventHandler(CollisionWall, "Wall");
	SetCollsionEventHandler(CollisionWallBullet, "Wall", "Bullet");
	SetCollsionEventHandler(CollisionExplosion, "Explosion");
	SetCollsionEventHandler(CollisionPlayerBullet, "Player", "Bullet");
	SetCollsionEventHandler(CollisionBulletPlayer, "Bullet", "Player");
}

EventHandler::~EventHandler()
{
}

void EventHandler::SetCollsionEventHandler(void (*_func)(GameObject*, GameObject*, CollisionEvent*), const std::string& _srcLayerName, const std::string& _dstLayerName)
{
	int srcLayer = (_srcLayerName.empty() ? -1 : LayerManager::GetInstance().GetLayerInd(_srcLayerName));
	int dstLayer = (_dstLayerName.empty() ? -1 : LayerManager::GetInstance().GetLayerInd(_dstLayerName));

	for (int i = (srcLayer == -1 ? 0 : srcLayer); i <= (srcLayer == -1 ? LayerManager::maxLayerInd : srcLayer); ++i)
	{
		for (int j = (dstLayer == -1 ? 0 : dstLayer); j <= (dstLayer == -1 ? LayerManager::maxLayerInd : dstLayer); ++j)
		{
			collisionEventHandler[i][j] = _func;
		}
	}
}

void EventHandler::CollisionNull(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent)
{

}

void EventHandler::CollisionWall(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent)
{
	RigidbodyComp* r = _dst->GetComponent<RigidbodyComp>();
	if (!r) return;

	r->CorrectPosByAABB(_dst->GetComponent<ColliderComp>(), _src->GetComponent<ColliderComp>());
}

void EventHandler::CollisionWallBullet(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent)
{
	RigidbodyComp* r = _dst->GetComponent<RigidbodyComp>();
	if (!r) return;

	r->CorrectPosByAABB(_dst->GetComponent<ColliderComp>(), _src->GetComponent<ColliderComp>());
	
	if (_colEvent->enter)
	{
		BulletComp* b = _dst->GetComponent<BulletComp>();
		if (b != nullptr)
		{
			b->Reflection(r->GetColDir());
		}
	}
}

void EventHandler::CollisionExplosion(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent)
{
	LifeComp* l = _dst->GetComponent<LifeComp>();
	if (l != nullptr)
	{
		l->AddLife(-1);
	}
}

void EventHandler::CollisionPlayerBullet(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent)
{
	BulletComp* b = _dst->GetComponent<BulletComp>();
	LifeComp* l = _dst->GetComponent<LifeComp>();

	if (!b || !l)
		return;

	if (b != nullptr && _colEvent->exit && !b->onAttack)
	{
		b->onAttack = true;
	}

	else if (l != nullptr && b->onAttack && _colEvent->enter)
	{
		l->AddLife(-1);
	}
}

void EventHandler::CollisionBulletPlayer(GameObject* _src, GameObject* _dst, CollisionEvent* _colEvent)
{
	LifeComp* l = _dst->GetComponent<LifeComp>();
	BulletComp* b = _src->GetComponent<BulletComp>();

	if (l == nullptr || b == nullptr)
		return;

	if (b->onAttack && _colEvent->enter)
	{
		l->AddLife(-1);
	}
}

void EventHandler::OnEvent(Event* _event)
{
	CollisionEvent* colEvent = dynamic_cast<CollisionEvent*>(_event);
	if (colEvent != nullptr)
	{
		GameObject* src = colEvent->src;
		GameObject* dst = colEvent->dst;

		collisionEventHandler[src->GetLayer()][dst->GetLayer()](src, dst, colEvent);
	}
}

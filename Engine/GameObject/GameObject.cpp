#include "GameObject.h"

#include "../Components.h"
#include "../Event/Events.h"

GameObject::GameObject(std::string _name) : name(_name), prefabName(), selected(false)
{
	SetLayer(LayerManager::maxLayerInd);
}

GameObject::~GameObject()
{
	for (auto& it : component)
	{
		if (it.second)
			delete it.second;
	}

	component.clear();
}

void GameObject::OnEvent(Event* _event)
{
	CollisionEvent* colEvent = dynamic_cast<CollisionEvent*>(_event);
	if (colEvent != nullptr)
	{
		GameObject* src = static_cast<GameObject*>(colEvent->src);
		GameObject* dst = static_cast<GameObject*>(colEvent->dst);

		// 나중에 function pointer array로 변경하기
		if (src->GetLayerName().compare("Wall") == 0)
		{
			RigidbodyComp* r = GetComponent<RigidbodyComp>();
			if (!r) return;

			r->CorrectPosByAABB(GetComponent<ColliderComp>(), src->GetComponent<ColliderComp>());
			
			if (layerName.compare("Bullet") == 0 && colEvent->enter)
			{
				BulletComp* b = GetComponent<BulletComp>();
				if (b != nullptr)
				{
					b->Reflection(r->GetColDir());
				}
			}
		}

		else if (src->GetLayerName().compare("Explosion") == 0)
		{
			LifeComp* l = GetComponent<LifeComp>();
			if (l != nullptr)
			{
				l->AddLife(-1);
			}
		}

		else if (layerName.compare("Player") == 0 && src->GetLayerName().compare("Bullet") == 0)
		{
			LifeComp* l = GetComponent<LifeComp>();
			if (l != nullptr && src->GetComponent<BulletComp>()->onAttack && colEvent->enter)
			{
				l->AddLife(-1);
			}
		}

		else if (layerName.compare("Bullet") == 0 && src->GetLayerName().compare("Player") == 0)
		{
			BulletComp* b = GetComponent<BulletComp>();
			LifeComp* l = GetComponent<LifeComp>();

			if (!b || !l)
				return;

			if (b != nullptr && colEvent->exit && !b->onAttack)
			{
				b->onAttack = true;
			}

			else if (l != nullptr && b->onAttack && colEvent->enter)
			{
				l->AddLife(-1);
			}
		}
	}
}

bool GameObject::IsHaveComponent(const std::string& _typeName)
{
	return component.find(_typeName) != component.end();
}

void GameObject::SetLayer(const int& _layer)
{
	layer = _layer;
	layerName = LayerManager::GetInstance().GetName(_layer);

	SpriteComp* s = GetComponent<SpriteComp>();
	if (s)
	{
		s->SetDepth();
	}
}
#include "ColliderComp.h"

#include "../CollisionManager/CollisionManager.h"
#include "../EventManager/EventManager.h"

#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../GraphicComponent/SpriteComp.h"
#include "../Event/CollisionEvent.h"

ColliderComp::ColliderComp(GameObject* _owner) : EngineComponent(_owner), pos(), scale(), rot(0)
{
	CollisionManager::GetInstance().AddCollider(this);
	SetCollider();
}

ColliderComp::~ColliderComp()
{
	CollisionManager::GetInstance().DeleteCollider(this);
}

void ColliderComp::Update()
{
	SetCollider();
}

bool ColliderComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		if (ImGui::Button("Delete Component"))
		{
			owner->DeleteComponent<ColliderComp>();
			ImGui::TreePop();
			return false;
		}

		ImGui::TreePop();
	}

	return true;
}

void ColliderComp::OnEvent(Event* _event)
{
	CollisionEvent* colEvent = dynamic_cast<CollisionEvent*>(_event);
	
	if (colEvent != nullptr)
	{
		RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
		if (r != nullptr)
		{
			r->colliders.push(static_cast<ColliderComp*>(_event->src));
		}
	}
}

void ColliderComp::SetPos(const glm::vec2& _otherPos)
{
	this->pos = _otherPos;
}

void ColliderComp::SetScale(const glm::vec2& _otherScale)
{
	this->scale = _otherScale;
}

void ColliderComp::SetRot(const float& _otherRot)
{
	this->rot = _otherRot;
}

void ColliderComp::SetCollider()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	pos.x = t->GetPos().x;
	pos.y = t->GetPos().y;

	scale.x = abs(t->GetScale().x);
	scale.y = abs(t->GetScale().y);

	rot = t->GetRot();
}

void ColliderComp::LoadFromJson(const json& _data)
{
	// Check how you saved, load from there
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto p = compData->find("position");
		pos.x = p->begin().value();
		pos.y = (p->begin() + 1).value();

		auto s = compData->find("scale");
		scale.x = s->begin().value();
		scale.y = (s->begin() + 1).value();

		auto r = compData->find("rotation");
		rot = r.value();
	}
}

json ColliderComp::SaveToJson()
{
	json data;

	data["type"] = TypeName;

	json compData;
	compData["position"] = { pos.x, pos.y };
	compData["scale"] = { scale.x, scale.y };
	compData["rotation"] = rot;

	data["compData"] = compData;
	return data;
}

BaseRTTI* ColliderComp::CreateColliderComponent(GameObject* _owner)
{
	BaseRTTI* p = new ColliderComp(_owner);
	return _owner->AddComponent<ColliderComp>(static_cast<BaseComponent*>(p));
}


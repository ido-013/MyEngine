#include "ColliderComp.h"

#include "../CollisionManager/CollisionManager.h"
#include "../EventManager/EventManager.h"

#include "../LogicComponent/LifeComp.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../GraphicComponent/SpriteComp.h"
#include "../Event/CollisionEvent.h"

void ColliderComp::CalculateMatrix()
{
	//Create a translate matrix
	glm::mat4 translateMtx(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		pos.x, pos.y, 0.f, 1.f
	);

	//Create a rotation matrix
	float angle = glm::radians(rot);
	float c = glm::cos(angle);
	float s = glm::sin(angle);
	glm::mat4 rotationMtx(
		c, s, 0.f, 0.f,
		-s, c, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	//Create a scale matrix
	glm::mat4 scaleMtx(
		scale.x / 2, 0.f, 0.f, 0.f,
		0.f, scale.y / 2, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	//Concatenate them
	colliderMatrix = translateMtx * rotationMtx * scaleMtx;
}

ColliderComp::ColliderComp(GameObject* _owner) : EngineComponent(_owner), pos(), scale(), rot(0)
{
	CollisionManager::GetInstance().AddCollider(this);
	SetCollider();
	CalculateMatrix();
}

ColliderComp::~ColliderComp()
{
	CollisionManager::GetInstance().DeleteCollider(this);
}

void ColliderComp::Update()
{
	SetCollider();
	CalculateMatrix();
}

bool ColliderComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		if (DeleteCompButton<ColliderComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<ColliderComp>())
		return false; 

	return true;
}

void ColliderComp::OnEvent(Event* _event)
{
	CollisionEvent* colEvent = dynamic_cast<CollisionEvent*>(_event);
	
	if (colEvent != nullptr)
	{
		if (!colEvent->isPass)
		{
			RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
			if (r != nullptr)
			{
				r->colliders.push(static_cast<ColliderComp*>(colEvent->src));
			}
		}
		else if (colEvent->enter)
		{
			LifeComp* l = owner->GetComponent<LifeComp>();
			if (l != nullptr)
			{
				l->AddLife(-1);
			}
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
		auto it = compData->find("position");
		pos.x = it->begin().value();
		pos.y = (it->begin() + 1).value();

		it = compData->find("scale");
		scale.x = it->begin().value();
		scale.y = (it->begin() + 1).value();

		it = compData->find("rotation");
		rot = it.value();
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
	return _owner->AddComponent<ColliderComp>();
}


#include "BulletComp.h"

#include "../LogicComponent/LifeComp.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"

BulletComp::BulletComp(GameObject* _owner) : LogicComponent(_owner), speed(150), bounce(1), onAttack(false)
{
}

BulletComp::~BulletComp()
{
}

void BulletComp::Update()
{
}

bool BulletComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat("Speed", &speed);

		ImGui::InputInt("Bounce", &bounce);

		ImGui::Separator();

		if (DeleteCompButton<BulletComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<BulletComp>())
		return false;

	return true;
}

void BulletComp::Fire(const float& _angle)
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
	if (!r) return;

	float radAngle = glm::radians(_angle);

	t->SetRot(_angle);
	r->SetVelocity(speed * cos(radAngle), speed * sin(radAngle));
}

void BulletComp::Reflection(const Direction& _dir)
{
	if (bounce == 0)
	{
		LifeComp* l = owner->GetComponent<LifeComp>();
		if (l != nullptr)
		{
			l->AddLife(-1);
		}
	}

	else
	{
		bounce--;

		TransformComp* t = owner->GetComponent<TransformComp>();
		if (!t) return;

		RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
		if (!r) return;

		glm::vec2 vel = r->GetVelocity();

		switch (_dir)
		{
		case UP:
		case DOWN:
			r->SetVelocity(vel.x, -vel.y);
			t->SetRot(glm::degrees(atan2f(-vel.y, vel.x)));
			break;
		case RIGHT:
		case LEFT:
			r->SetVelocity(-vel.x, vel.y);
			t->SetRot(glm::degrees(atan2f(vel.y, -vel.x)));
			break;
		}	
	}
}

void BulletComp::LoadFromJson(const json& _data)
{
	// Check how you saved, load from there
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("speed");
		speed = it.value();

		it = compData->find("bounce");
		bounce = it.value();
	}
}

json BulletComp::SaveToJson()
{
	json data;

	data["type"] = TypeName;

	json compData;
	compData["speed"] = speed;
	compData["bounce"] = bounce;

	data["compData"] = compData;
	return data;
}

BaseRTTI* BulletComp::CreateBulletComponent(GameObject* _owner)
{
	return _owner->AddComponent<BulletComp>();
}


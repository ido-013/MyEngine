#include "BulletComp.h"

#include "../EngineComponent/RigidbodyComp.h"

BulletComp::BulletComp(GameObject* _owner) : LogicComponent(_owner), speed(150), bounce(0), maxBounce(1), angle(0)
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

		ImGui::InputInt("Max Bounce", &maxBounce);

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
	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
	if (!r) return;

	angle = _angle;

	r->SetVelocity(speed * cos(angle), speed * sin(angle));
}

void BulletComp::LoadFromJson(const json& _data)
{
	// Check how you saved, load from there
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("speed");
		speed = it.value();

		it = compData->find("maxBounce");
		maxBounce = it.value();
	}
}

json BulletComp::SaveToJson()
{
	json data;

	data["type"] = TypeName;

	json compData;
	compData["speed"] = speed;
	compData["maxBounce"] = maxBounce;

	data["compData"] = compData;
	return data;
}

BaseRTTI* BulletComp::CreateBulletComponent(GameObject* _owner)
{
	return _owner->AddComponent<BulletComp>();
}


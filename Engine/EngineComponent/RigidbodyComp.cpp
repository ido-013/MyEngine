#include "RigidbodyComp.h"

#include "TransformComp.h"
#include "../OpenGL/GLHelper.h"

#include <iostream>

bool RigidbodyComp::CheckEpsilon(float value)
{
	return abs(value) < 0.001;
}

RigidbodyComp::RigidbodyComp(GameObject* _owner) : EngineComponent(_owner), velocity(), maxVelocity()
{
	velocity.x = 0;
	velocity.y = 0;
	maxVelocity.x = 500;
	maxVelocity.y = 500;

	acceleration.x = 0;
	acceleration.y = 0;
	maxAcceleration.x = 500;
	maxAcceleration.y = 500;
}

RigidbodyComp::~RigidbodyComp()
{

}

void RigidbodyComp::AddVelocity(float _x, float _y)
{
	velocity.x += _x;
	velocity.y += _y;
}

void RigidbodyComp::SetVelocity(float _x, float _y)
{
	velocity.x = _x;
	velocity.y = _y;
}


void RigidbodyComp::ClearVelocity()
{
	velocity.x = 0;
	velocity.y = 0;
}

void RigidbodyComp::AddAcceleration(float _x, float _y)
{
	acceleration.x += _x;
	acceleration.y += _y;
}

void RigidbodyComp::SetAcceleration(float _x, float _y)
{
	acceleration.x = _x;
	acceleration.y = _y;
}

void RigidbodyComp::ClearAcceleration()
{
	acceleration.x = 0;
	acceleration.y = 0;
}

void RigidbodyComp::Update()
{
	//Get the transform
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t)	return;

	float dt = (float)GLHelper::delta_time;

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;

	float x = t->GetPos().x + velocity.x * dt;
	float y = t->GetPos().y + velocity.y * dt;

	velocity.x /= drag;
	velocity.y /= drag;

	//If im too low, just set to 0
	if (CheckEpsilon(velocity.x))
		velocity.x = 0;

	if (CheckEpsilon(velocity.y))
		velocity.y = 0;

	t->SetPos({ x, y });
}

bool RigidbodyComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat2("Velocity", &velocity[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::InputFloat2("MaxVelocity", &maxVelocity[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::InputFloat2("Acceleration", &acceleration[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::InputFloat2("MaxAcceleration", &maxAcceleration[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::InputFloat("Drag", &drag);

		if (ImGui::Button("Delete Component"))
		{
			owner->DeleteComponent<RigidbodyComp>();
			ImGui::TreePop();
			return false;
		}

		ImGui::TreePop();
	}

	return true;
}

void RigidbodyComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("velocity");
		velocity.x = it->begin().value();
		velocity.y = (it->begin() + 1).value();

		it = compData->find("maxVelocity");
		maxVelocity.x = it->begin().value();
		maxVelocity.y = (it->begin() + 1).value();

		it = compData->find("acceleration");
		acceleration.x = it->begin().value();
		acceleration.y = (it->begin() + 1).value();

		it = compData->find("maxAcceleration");
		maxAcceleration.x = it->begin().value();
		maxAcceleration.y = (it->begin() + 1).value();

		it = compData->find("drag");
		drag = it.value();
	}
}

json RigidbodyComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["velocity"] = { velocity.x, velocity.y };
	compData["maxVelocity"] = { maxVelocity.x, maxVelocity.y };
	compData["acceleration"] = { acceleration.x, acceleration.y };
	compData["maxAcceleration"] = { maxAcceleration.x, maxAcceleration.y };
	compData["drag"] = drag;

	data["compData"] = compData;

	return data;
}

BaseRTTI* RigidbodyComp::CreateRigidBodyComponent(GameObject* _owner)
{
	BaseRTTI* p = new RigidbodyComp(_owner);
	_owner->AddComponent<RigidbodyComp>(static_cast<BaseComponent*>(p));
	return p;
}
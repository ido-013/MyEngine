#include "RigidbodyComp.h"

#include "TransformComp.h"
#include "../OpenGL/GLHelper.h"

#include <iostream>

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

void RigidbodyComp::AddVelocity(float x, float y)
{
	velocity.x += x;
	velocity.y += y;
}

void RigidbodyComp::SetVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}


void RigidbodyComp::ClearVelocity()
{
	velocity.x = 0;
	velocity.y = 0;
}

void RigidbodyComp::AddAcceleration(float x, float y)
{
	acceleration.x += x;
	acceleration.y += y;
}

void RigidbodyComp::SetAcceleration(float x, float y)
{
	acceleration.x = x;
	acceleration.y = y;
}

void RigidbodyComp::ClearAcceleration()
{
	acceleration.x = 0;
	acceleration.y = 0;
}

void RigidbodyComp::Update()
{
	//float dt = (float)AEFrameRateControllerGetFrameTime();
	float dt = GLHelper::delta_time;

	//Get the transform
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t)	return;

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;

	float x = t->GetPos().x + velocity.x * dt;
	float y = t->GetPos().y + velocity.y * dt;

	velocity.x /= drag;
	velocity.y /= drag;

	////If im too low, just set to 0
	/*if (CheckEpsilon(velocity.x) == false)
		velocity.x = 0;

	if (CheckEpsilon(velocity.y) == false)
		velocity.y = 0;*/

	t->SetPos({ x, y });
}

void RigidbodyComp::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");

	if (compData != data.end())
	{
		auto it = compData->find("velocity");
		velocity.x = it->begin().value();
		velocity.y = (it->begin() + 1).value();

		it = compData->find("maxVelocity");
		maxVelocity.x = it->begin().value();
		maxVelocity.y = (it->begin() + 1).value();
	}
}

json RigidbodyComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["velocity"] = { velocity.x, velocity.y };
	compData["maxVelocity"] = { maxVelocity.x, maxVelocity.y };
	data["compData"] = compData;

	return data;
}

BaseRTTI* RigidbodyComp::CreateRigidBodyComponent(GameObject* owner)
{
	BaseRTTI* p = new RigidbodyComp(owner);
	owner->AddComponent<RigidbodyComp>(static_cast<BaseComponent*>(p));
	return p;
}
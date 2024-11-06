#include "RigidbodyComp.h"

#include <iostream>

#include "../OpenGL/GLHelper.h"

#include "TransformComp.h"
#include "ColliderComp.h"

bool RigidbodyComp::CheckEpsilon(float value)
{
	return abs(value) < 0.001;
}

void RigidbodyComp::CorrectPosByAABB(const ColliderComp* _col, const ColliderComp* _otherCol)
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	glm::vec2 otherColPos = _otherCol->GetPos();
	glm::vec2 otherColScale = _otherCol->GetScale();
	glm::vec2 colPos = _col->GetPos();
	glm::vec2 colScale = _col->GetScale();

	float x = t->GetPos().x;
	float y = t->GetPos().y;

	float dis[4] =
	{
		abs(otherColPos.x + otherColScale.x / 2 - (colPos.x - colScale.x / 2)), //LEFT
		abs(otherColPos.x - otherColScale.x / 2 - (colPos.x + colScale.x / 2)), //RIGHT
		abs(otherColPos.y + otherColScale.y / 2 - (colPos.y - colScale.y / 2)),	//UP
		abs(otherColPos.y - otherColScale.y / 2 - (colPos.y + colScale.y / 2))	//DOWN
	};

	float minDis = dis[0];
	colDir = LEFT;

	for (int i = 1; i < 4; i++)
	{
		if (minDis > dis[i])
		{
			minDis = dis[i];
			colDir = Direction(i);
		}
	}

	switch (colDir)
	{
	case LEFT:
		x = otherColPos.x + otherColScale.x / 2 + colScale.x / 2 + 0.1f;
		break;
	case RIGHT:
		x = otherColPos.x - otherColScale.x / 2 - colScale.x / 2 - 0.1f;
		break;
	case UP:
		y = otherColPos.y + otherColScale.y / 2 + colScale.y / 2 + 0.1f;
		break;
	case DOWN:
		y = otherColPos.y - otherColScale.y / 2 - colScale.y / 2 - 0.1f;
		break;
	}

	t->SetPos({ x, y });
}

RigidbodyComp::RigidbodyComp(GameObject* _owner) : EngineComponent(_owner), drag(1.01f), onDrag(false), velocity(), maxVelocity(), colDir()
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
	TransformComp* tf = owner->GetComponent<TransformComp>();
	if (!tf) return;

	float dt = (float)GLHelper::delta_time;

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;

	float x = tf->GetPos().x + velocity.x * dt;
	float y = tf->GetPos().y + velocity.y * dt;

	if (onDrag)
	{
		velocity.x /= drag;
		velocity.y /= drag;
	}

	//If im too low, just set to 0
	if (CheckEpsilon(velocity.x))
		velocity.x = 0;

	if (CheckEpsilon(velocity.y))
		velocity.y = 0;

	//ColliderComp* col = owner->GetComponent<ColliderComp>();

	//if (col != nullptr)
	//{
	//	//resolution
	//	while (!colliders.empty())
	//	{
	//		ColliderComp* otherCol = colliders.front();
	//		colliders.pop();

	//		CorrectPosByAABB(col, otherCol, x, y);
	//	}
	//}

	tf->SetPos({ x, y });
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

		ImGui::Checkbox("On Drag", &onDrag);

		ImGui::Separator();

		if (DeleteCompButton<RigidbodyComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<RigidbodyComp>())
		return false;

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
		
		it = compData->find("onDrag");
		onDrag = it.value();
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
	compData["onDrag"] = onDrag;

	data["compData"] = compData;

	return data;
}

BaseRTTI* RigidbodyComp::CreateRigidBodyComponent(GameObject* _owner)
{
	return _owner->AddComponent<RigidbodyComp>();
}
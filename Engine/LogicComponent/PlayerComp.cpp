#include "PlayerComp.h"

#include <string>

#include "../OpenGL/GLHelper.h"

#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../GraphicComponent/SpriteComp.h"

PlayerComp::PlayerComp(GameObject* _owner) : LogicComponent(_owner)
{

}

PlayerComp::~PlayerComp()
{

}

void PlayerComp::Update()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
	if (!r) return;

	r->ClearVelocity();

	if (GLHelper::keyState[GLFW_KEY_UP])
	{
		r->AddVelocity(0, speed);
	}

	if (GLHelper::keyState[GLFW_KEY_LEFT])
	{
		r->AddVelocity(-speed, 0);
	}

	if (GLHelper::keyState[GLFW_KEY_DOWN])
	{
		r->AddVelocity(0, -speed);
	}

	if (GLHelper::keyState[GLFW_KEY_RIGHT])
	{
		r->AddVelocity(speed, 0);
	}
}

bool PlayerComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat("Speed", &speed);

		if (ImGui::Button("Delete Component"))
		{
			owner->DeleteComponent<PlayerComp>();
			ImGui::TreePop();
			return false;
		}

		ImGui::TreePop();
	}

	return true;
}

void PlayerComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("speed");
		speed = it.value();
	}
}

json PlayerComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["speed"] = speed;
	data["compData"] = compData;

	return data;
}


BaseRTTI* PlayerComp::CreatePlayerComponent(GameObject* _owner)
{
	BaseRTTI* p = new PlayerComp(_owner);
	_owner->AddComponent<PlayerComp>(static_cast<BaseComponent*>(p));
	return p;
}


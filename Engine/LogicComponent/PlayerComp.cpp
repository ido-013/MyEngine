#include "PlayerComp.h"

#include <string>

#include "../OpenGL/GLHelper.h"
#include "../Editor/Util.h"
#include "../Prefab/Prefab.h"

#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../GraphicComponent/SpriteComp.h"

PlayerComp::PlayerComp(GameObject* _owner) : LogicComponent(_owner), speed(100)
{
	keyCode[UP] = GLFW_KEY_UP;
	keyCode[DOWN] = GLFW_KEY_DOWN;
	keyCode[LEFT] = GLFW_KEY_LEFT;
	keyCode[RIGHT] = GLFW_KEY_RIGHT;
	keyCode[BOMB] = GLFW_KEY_Z;
	keyCode[BULLET] = GLFW_KEY_X;
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

	if (GLHelper::keyState[keyCode[UP]])
	{
		r->AddVelocity(0, speed);
	}

	if (GLHelper::keyState[keyCode[LEFT]])
	{
		r->AddVelocity(-speed, 0);
	}

	if (GLHelper::keyState[keyCode[DOWN]])
	{
		r->AddVelocity(0, -speed);
	}

	if (GLHelper::keyState[keyCode[RIGHT]])
	{
		r->AddVelocity(speed, 0);
	}

	if (GLHelper::keyState[keyCode[BOMB]])
	{
		if (!bombName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Bomb", bombName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
			GLHelper::keyState[keyCode[BOMB]] = GL_FALSE;
		}
	}
	
	if (GLHelper::keyState[keyCode[BULLET]])
	{
		if (!bulletName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Bullet", bulletName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
			GLHelper::keyState[keyCode[BULLET]] = GL_FALSE;
		}
	}
}

bool PlayerComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat("Speed", &speed);

		FileSelectComboOnce(bombName, "Bomb", bombName, "Assets/Prefab", ".prefab");

		FileSelectComboOnce(bulletName, "Bullet", bulletName, "Assets/Prefab", ".prefab");

		if (ImGui::TreeNode("Key Setting"))
		{
			KeyChangePopup("Up:", keyCode[UP]);
			KeyChangePopup("Down:", keyCode[DOWN]);
			KeyChangePopup("Left:", keyCode[LEFT]);
			KeyChangePopup("Right:", keyCode[RIGHT]);
			KeyChangePopup("Bomb:", keyCode[BOMB]);
			KeyChangePopup("Bullet:", keyCode[BULLET]);

			ImGui::TreePop();
		}

		if (DeleteCompButton<PlayerComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<PlayerComp>())
		return false;

	return true;
}

void PlayerComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("speed");
		speed = it.value();

		it = compData->find("keyCode");
		for (int i = 0; i < 6; i++)
		{
			keyCode[i] = (it->begin() + i).value();
		}

		it = compData->find("bombName");
		bombName = it.value();

		it = compData->find("bulletName");
		bulletName = it.value();
	}
}

json PlayerComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["speed"] = speed;
	compData["keyCode"] = keyCode;
	compData["bombName"] = bombName;
	compData["bulletName"] = bulletName;

	data["compData"] = compData;

	return data;
}

BaseRTTI* PlayerComp::CreatePlayerComponent(GameObject* _owner)
{
	return _owner->AddComponent<PlayerComp>();
}


#include "PlayerComp.h"

#include <string>

#include "../EventManager/EventManager.h"
#include "../OpenGL/GLHelper.h"
#include "../Editor/Util.h"
#include "../Prefab/Prefab.h"

#include "BulletComp.h"
#include "EnemyComp.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../GraphicComponent/SpriteComp.h"

#include "../Event/Events.h"

PlayerComp::PlayerComp(GameObject* _owner) : LogicComponent(_owner), speed(100),
											 isBombCooldown(false), bombCooldown(0), maxBombCooldown(3.f),
											 isBulletCooldown(false), bulletCooldown(0), maxBulletCooldown(0.2f)
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
	EventManager::GetInstance().AddEvent<PlayerCreateBombEvent>(new PlayerCreateBombEvent(GetOwner(), nullptr, obj), "Enemy");
}

void PlayerComp::Update()
{
	Move();

	AttackBomb();
	AttackBullet();

	CheckCooldown(isBombCooldown, bombCooldown, maxBombCooldown);
	CheckCooldown(isBulletCooldown, bulletCooldown, maxBulletCooldown);
}

bool PlayerComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat("Speed", &speed);
		ImGui::InputFloat("Bomb Cooldown", &maxBombCooldown);
		ImGui::InputFloat("Bullet Cooldown", &maxBulletCooldown);

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

		ImGui::Separator();

		if (DeleteCompButton<PlayerComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<PlayerComp>())
		return false;

	return true;
}

void PlayerComp::Move()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
	if (!r) return;

	float rot = t->GetRot();
	

	r->ClearVelocity();

	if (GLHelper::keyState[keyCode[UP]])
	{
		float angle = glm::radians(rot);
		r->AddVelocity(cos(angle) * speed, sin(angle) * speed);
	}

	if (GLHelper::keyState[keyCode[LEFT]])
	{
		t->SetRot(rot + 0.5f);
	}

	if (GLHelper::keyState[keyCode[DOWN]])
	{
		float angle = glm::radians(rot);
		r->AddVelocity(-cos(angle) * speed, -sin(angle) * speed);
	}

	if (GLHelper::keyState[keyCode[RIGHT]])
	{
		t->SetRot(rot - 0.5f);
	}
}

void PlayerComp::AttackBomb()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	if (!isBombCooldown && GLHelper::keyState[keyCode[BOMB]])
	{
		isBombCooldown = true;

		if (!bombName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Bomb", bombName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
			EventManager::GetInstance().AddEvent<PlayerCreateBombEvent>(new PlayerCreateBombEvent(GetOwner(), nullptr, obj), "Enemy");
		}
	}
}

void PlayerComp::AttackBullet()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	if (!isBulletCooldown && GLHelper::keyState[keyCode[BULLET]])
	{
		isBulletCooldown = true;

		if (!bulletName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Bullet", bulletName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
			obj->GetComponent<BulletComp>()->Fire(t->GetRot());
			EventManager::GetInstance().AddEvent<PlayerCreateBulletEvent>(new PlayerCreateBulletEvent(GetOwner(), nullptr, obj), "Enemy");
		}
	}
}

void PlayerComp::CheckCooldown(bool& _isCooldown, float& _cooldown, const float& _maxCooldown)
{
	if (_isCooldown)
	{
		_cooldown += (float)GLHelper::delta_time;

		if (_cooldown > _maxCooldown)
		{
			_cooldown = 0;
			_isCooldown = false;
		}
	}
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

		it = compData->find("bombCooldown");
		maxBombCooldown = it.value();

		it = compData->find("bulletName");
		bulletName = it.value();
		
		it = compData->find("bulletCooldown");
		maxBulletCooldown = it.value();
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
	compData["bombCooldown"] = maxBombCooldown;

	compData["bulletName"] = bulletName;
	compData["bulletCooldown"] = maxBulletCooldown;

	data["compData"] = compData;

	return data;
}

BaseRTTI* PlayerComp::CreatePlayerComponent(GameObject* _owner)
{
	return _owner->AddComponent<PlayerComp>();
}


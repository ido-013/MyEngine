#include "EnemyComp.h"

#include "../OpenGL/GLHelper.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Prefab/Prefab.h"
#include "../Editor/Grid.h"
#include "../Pathfinder/Pathfinder.h"

#include "../Editor/Util.h"

#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "BulletComp.h"

void EnemyComp::Move()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();

	if (waypoint.empty())
	{
		UpdateWaypoint();
	}
	else
	{
		// nearby target -> change pos and pop waypoint
		if ((abs(waypoint.front().x - t->GetPos().x) + abs(waypoint.front().y - t->GetPos().y)) < 3)
		{
			t->SetPos(waypoint.front());
			waypoint.pop();
		}

		// rotation and move
		else if (RotationToTarget(waypoint.front()))
		{
			float angle = glm::radians(t->GetRot());
			r->SetVelocity(cos(angle) * speed, sin(angle) * speed);
		}
	}
}

bool EnemyComp::RotationToTarget(const glm::vec2& _targetPos)
{
	TransformComp* t = owner->GetComponent<TransformComp>();

	float targetRot = glm::degrees(atan2f(_targetPos.y - t->GetPos().y, _targetPos.x - t->GetPos().x));

	float rotDiff = abs(targetRot - t->GetRot()) < abs(targetRot - (360 + t->GetRot())) ? targetRot - t->GetRot() : targetRot - (360 + t->GetRot());

	if (abs(rotDiff) > 3)
	{
		t->SetRot(t->GetRot() + (rotDiff < 0 ? -1 : 1));
		return false;
	}

	return true;
}

void EnemyComp::AttackBomb()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	if (!isBombCooldown)
	{
		isBombCooldown = true;

		if (!bombName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Bomb", bombName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
		}
	}
}

void EnemyComp::AttackBullet()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	if (!isBulletCooldown)
	{
		isBulletCooldown = true;

		if (!bulletName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Bullet", bulletName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
			obj->GetComponent<BulletComp>()->Fire(t->GetRot());
		}
	}
}

void EnemyComp::CheckCooldown(bool& _isCooldown, float& _cooldown, const float& _maxCooldown)
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

EnemyComp::EnemyComp(GameObject* _owner) : LogicComponent(_owner), speed(100),
										   isBombCooldown(false), bombCooldown(0), maxBombCooldown(10.f),
										   isBulletCooldown(false), bulletCooldown(0), maxBulletCooldown(1.f),
										   player(nullptr), playerPos()
{
}

EnemyComp::~EnemyComp()
{
}

void EnemyComp::Update()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
	if (!r) return;

	r->ClearVelocity();

	if (player == nullptr)
	{
		player = GameObjectManager::GetInstance().GetObject("Player");
		return;
	}
	else
	{
		playerPos = player->GetComponent<TransformComp>()->GetPos();
	}

	glm::vec2 gridInd = Grid::GetInstance().GetGridInd(t->GetPos());
	glm::vec2 playerGridInd = Grid::GetInstance().GetGridInd(playerPos);

	if (Pathfinder::GetInstance().CheckStraightLine((int)gridInd.x, (int)gridInd.y, (int)playerGridInd.x, (int)playerGridInd.y))
	{
		if (RotationToTarget(playerPos))
		{
			AttackBullet();
		}
	}
	else
	{
		Move();
		AttackBomb();
	}

	CheckCooldown(isBombCooldown, bombCooldown, maxBombCooldown);
	CheckCooldown(isBulletCooldown, bulletCooldown, maxBulletCooldown);
}

bool EnemyComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat("Speed", &speed);
		ImGui::InputFloat("Bomb Cooldown", &maxBombCooldown);
		ImGui::InputFloat("Bullet Cooldown", &maxBulletCooldown);

		FileSelectComboOnce(bombName, "Bomb", bombName, "Assets/Prefab", ".prefab");
		FileSelectComboOnce(bulletName, "Bullet", bulletName, "Assets/Prefab", ".prefab");

		ImGui::Separator();

		if (DeleteCompButton<EnemyComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<EnemyComp>())
		return false;

	return true;
}

void EnemyComp::SetPlayerNull()
{
	player = nullptr;
	playerPos = glm::vec2();
}

void EnemyComp::UpdateWaypoint()
{
	TransformComp* t = owner->GetComponent<TransformComp>();

	glm::vec2 gridInd = Grid::GetInstance().GetGridInd(t->GetPos());
	glm::vec2 playerGridInd = Grid::GetInstance().GetGridInd(playerPos);

	waypoint = Pathfinder::GetInstance().ComputePath((int)gridInd.x, (int)gridInd.y, (int)playerGridInd.x, (int)playerGridInd.y);
}

void EnemyComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("speed");
		speed = it.value();

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

json EnemyComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["speed"] = speed;

	compData["bombName"] = bombName;
	compData["bombCooldown"] = maxBombCooldown;

	compData["bulletName"] = bulletName;
	compData["bulletCooldown"] = maxBulletCooldown;

	data["compData"] = compData;

	return data;
}

BaseRTTI* EnemyComp::CreateEnemyComponent(GameObject* _owner)
{
	return _owner->AddComponent<EnemyComp>();
}

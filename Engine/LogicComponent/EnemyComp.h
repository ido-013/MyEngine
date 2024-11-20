#pragma once

#include <glm/glm.hpp>
#include <queue>

#include "LogicComponent.h"

class EnemyComp : public LogicComponent
{
private:
	float speed;

	std::string bombName;
	bool isBombCooldown;
	float bombCooldown;
	float maxBombCooldown;

	std::string bulletName;
	bool isBulletCooldown;
	float bulletCooldown;
	float maxBulletCooldown;

	GameObject* player;
	glm::vec2 playerPos;

	std::queue<glm::vec2> waypoint;

	void Move();

	bool RotationToTarget(const glm::vec2& _targetPos);

	void AttackBomb();
	void AttackBullet();

	void CheckCooldown(bool& _isCooldown, float& _cooldown, const float& _maxCooldown);

public:
	EnemyComp(GameObject* _owner);
	~EnemyComp();

	void Update() override;
	bool Edit() override;

	void SetPlayerNull();
	void UpdateWaypoint();

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateEnemyComponent(GameObject* _owner);

	static constexpr const char* TypeName = "EnemyComp";
};
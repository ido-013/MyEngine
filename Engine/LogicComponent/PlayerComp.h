#pragma once

#include <glm/glm.hpp>

#include "LogicComponent.h"

class PlayerComp : public LogicComponent
{
private:
	enum KeyTag
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
		BOMB,
		BULLET,
	};

	float speed;

	int keyCode[6];

	std::string bombName;
	bool isBombCooldown;	
	float bombCooldown;
	float maxBombCooldown;

	std::string bulletName;
	bool isBulletCooldown;
	float bulletCooldown;
	float maxBulletCooldown;

	glm::vec2 preGridInd;

	void Move();

	void AttackBomb();
	void AttackBullet();

	void CheckCooldown(bool& _isCooldown, float& _cooldown, const float& _maxCooldown);

public:
	PlayerComp(GameObject* _owner);
	~PlayerComp();

	void Update() override;
	bool Edit() override;

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreatePlayerComponent(GameObject* _owner);

	static constexpr const char* TypeName = "PlayerComp";
};
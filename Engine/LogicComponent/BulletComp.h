#pragma once

#include "LogicComponent.h"
#include "../Direction/Direction.h"

class BulletComp : public LogicComponent
{
private:
	float speed;

	int bounce;

public:
	bool onAttack;

	BulletComp(GameObject* _owner);
	~BulletComp();

	void Update() override;
	bool Edit() override;

	void Fire(const float& _angle);
	void Reflection(const Direction& _dir);

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateBulletComponent(GameObject* _owner);
	static constexpr const char* TypeName = "BulletComp";
};
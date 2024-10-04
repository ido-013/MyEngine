#pragma once
#include "LogicComponent.h"

class PlayerComp : public LogicComponent
{
private:
	float speed = 0.5f;

public:
	PlayerComp(GameObject* _owner);
	~PlayerComp();
	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static BaseRTTI* CreatePlayerComponent(GameObject* owner);

	static constexpr const char* TypeName = "PlayerComp";
};
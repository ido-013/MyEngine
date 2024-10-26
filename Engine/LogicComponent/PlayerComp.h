#pragma once
#include "LogicComponent.h"

class PlayerComp : public LogicComponent
{
private:
	float speed = 100;

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
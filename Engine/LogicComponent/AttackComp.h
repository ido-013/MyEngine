#pragma once

#include "LogicComponent.h"

#include <string>

class AttackComp : public LogicComponent
{
private:
	std::string prefabName;

public:
	AttackComp(GameObject* _owner);
	~AttackComp();

	void Update() override;
	bool Edit() override;

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateAttackComponent(GameObject* _owner);

	static constexpr const char* TypeName = "AttackComp";
};
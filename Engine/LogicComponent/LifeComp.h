#pragma once
#include "LogicComponent.h"

class LifeComp : public LogicComponent
{
private:
	int life;
	int maxLife;

	bool isImmune;
	float immuneTime;
	float timer;

public:
	LifeComp(GameObject* _owner);
	~LifeComp();

	void Update() override;
	bool Edit() override;

	void AddLife(const int& _value);

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateLifeComponent(GameObject* _owner);

	static constexpr const char* TypeName = "LifeComp";
};
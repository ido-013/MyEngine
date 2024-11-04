#pragma once
#include "EngineComponent.h"

class LifetimeComp : public EngineComponent
{
private:
	float lifetime;
	float currentTime;

public:
	LifetimeComp(GameObject* _owner);
	~LifetimeComp();

	void SetLifetime(const float& _lifetime) { lifetime = _lifetime; }

	void Update() override;
	bool Edit() override;

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateLifetimeComponent(GameObject* owner);
	static constexpr const char* TypeName = "LifetimeComp";
};
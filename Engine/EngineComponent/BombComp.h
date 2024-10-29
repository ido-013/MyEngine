#pragma once

#include <string>

#include "EngineComponent.h"

class BombComp : public EngineComponent
{
private:
	int dx[4] = { 0, -1, 1, 0 };
	int dy[4] = { -1, 0, 0, 1 };

	int length;
	float timer;
	float maxTimer;

	std::string effectName;

public:
	BombComp(GameObject* _owner);
	~BombComp();

	void Update() override;
	bool Edit() override;

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateBombComponent(GameObject* _owner);
	static constexpr const char* TypeName = "BombComp";
};
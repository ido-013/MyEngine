#pragma once

#include <string>
#include <glm/glm.hpp>

#include "LogicComponent.h"

class BombComp : public LogicComponent
{
private:
	int dx[4] = { 0, -1, 1, 0 };
	int dy[4] = { -1, 0, 0, 1 };

	glm::vec2 pos;
	glm::vec2 scale;
	int length;

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
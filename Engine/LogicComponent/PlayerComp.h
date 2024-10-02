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

	static constexpr const char* TypeName = "PlayerComp";
};
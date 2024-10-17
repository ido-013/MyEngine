#pragma once

#include <glm/glm.hpp>
#include "../EngineComponent/EngineComponent.h"

class RigidbodyComp : public EngineComponent
{
private:
	float drag = 1.01f;

	glm::vec2 velocity;
	glm::vec2 maxVelocity;
	glm::vec2 acceleration;
	glm::vec2 maxAcceleration;

	bool CheckEpsilon(float _value);

public:
	RigidbodyComp(GameObject* _owner);
	~RigidbodyComp();

	void AddVelocity(float _x, float _y);
	void SetVelocity(float _x, float _y);	
	void ClearVelocity();

	void AddAcceleration(float _x, float _y);
	void SetAcceleration(float _x, float _y);
	void ClearAcceleration();

	void Update() override;
	bool Edit() override;

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateRigidBodyComponent(GameObject* _owner);

	static constexpr const char* TypeName = "RigidbodyComp";
};
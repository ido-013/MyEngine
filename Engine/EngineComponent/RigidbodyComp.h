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

public:
	RigidbodyComp(GameObject* _owner);
	~RigidbodyComp();

	void AddVelocity(float x, float y);
	void SetVelocity(float x, float y);	
	void ClearVelocity();

	void AddAcceleration(float x, float y);
	void SetAcceleration(float x, float y);
	void ClearAcceleration();

	void Update() override;

	static constexpr const char* TypeName = "RigidbodyComp";
};
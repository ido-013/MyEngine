#pragma once

#include <queue>
#include <glm/glm.hpp>

#include "../Direction/Direction.h"

#include "../EngineComponent/EngineComponent.h"

class ColliderComp;

class RigidbodyComp : public EngineComponent
{
private:
	float drag;
	bool onDrag;

	glm::vec2 velocity;
	glm::vec2 maxVelocity;
	glm::vec2 acceleration;
	glm::vec2 maxAcceleration;

	//std::queue<ColliderComp*> colliders;
	Direction colDir;

	bool CheckEpsilon(float _value);

public:
	RigidbodyComp(GameObject* _owner);
	~RigidbodyComp();

	void Update() override;
	bool Edit() override;

	void AddVelocity(float _x, float _y);
	void SetVelocity(float _x, float _y);
	void ClearVelocity();
	const glm::vec2& GetVelocity() { return velocity; }

	void AddAcceleration(float _x, float _y);
	void SetAcceleration(float _x, float _y);
	void ClearAcceleration();

	//std::queue<ColliderComp*>& GetColliders() { return colliders; }
	void CorrectPosByAABB(const ColliderComp* _col, const ColliderComp* _otherCol);
	const Direction& GetColDir() const { return colDir; }

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateRigidBodyComponent(GameObject* _owner);

	static constexpr const char* TypeName = "RigidbodyComp";
};
#pragma once

#include <glm/glm.hpp>

#include "EngineComponent.h"
#include "../Event/Entity.h"

class ColliderComp : public EngineComponent
{
private:
	glm::vec2 pos;
	glm::vec2 scale;
	float rot;

	glm::mat4 colliderMatrix;

	void CalculateMatrix();

public:
	ColliderComp(GameObject* _owner);
	~ColliderComp();

	void Update() override;
	bool Edit() override;

	//Gettors
	const glm::vec2& GetPos() const { return pos; }
	const glm::vec2& GetScale() const { return scale; }
	const float& GetRot() const { return rot; }
	const glm::mat4& GetMatrix() const { return colliderMatrix; }

	//Mutators
	void SetPos(const glm::vec2& _otherPos);
	void SetScale(const glm::vec2& _otherScale);
	void SetRot(const float& _otherRot);

	void SetCollider();

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateColliderComponent(GameObject* _owner);
	static constexpr const char* TypeName = "ColliderComp";
};
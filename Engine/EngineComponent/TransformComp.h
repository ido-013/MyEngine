#pragma once
#include <glm/glm.hpp>
#include "EngineComponent.h"

class TransformComp : public EngineComponent
{
private:
	glm::vec2 pos;
	glm::vec2 scale;
	float rot;

	glm::mat3 transformMatrix;

	void CalculateMatrix();

public:
	TransformComp(GameObject* _owner);
	~TransformComp();

	void Update() override;

	//Gettors
	const glm::vec2& GetPos() const { return pos; }
	const glm::vec2& GetScale() const { return scale; }
	const float& GetRot() const { return rot; }
	const glm::mat3& GetMatrix() const { return transformMatrix; }

	//Mutators
	void SetPos(const glm::vec2& otherPos);
	void SetScale(const glm::vec2& otherScale);
	void SetRot(const float& otherRot);

	//Other Fn
	void PrintMatrix();

	static constexpr const char* TypeName = "TransformComp";
};
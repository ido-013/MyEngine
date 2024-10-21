#pragma once

#include <glm/glm.hpp>

class Camera
{
private:
	Camera();
	~Camera();

	Camera(const Camera&) = delete;
	const Camera& operator=(const Camera&) = delete;

	glm::vec2 pos;
	glm::vec2 editPos;
	glm::vec2 playPos;

	float height;
	float speed;

	glm::mat3 world_to_ndc_xform;

public:
	static Camera& GetInstance()
	{
		static Camera instance;
		return instance;
	}

	void Update();
	void Edit();

	void CalculateMatrix();
	glm::mat3& GetMatrix() { return world_to_ndc_xform; }
	
	const glm::vec2& GetPos();
	void SetPos(const float& _x, const float& _y);

	void SetHeight(const float& _value);
	void AddHeight(const float& _value);

	const float& GetHeight();
};
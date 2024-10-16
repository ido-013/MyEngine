#pragma once

#include <glm/glm.hpp>

class Camera
{
private:
	Camera();
	~Camera();

	Camera(const Camera& other) = delete;
	const Camera& operator=(const Camera& other) = delete;

	glm::vec2 pos;

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
	void Info();

	void CalculateMatrix();
	glm::mat3& GetMatrix() { return world_to_ndc_xform; }
	
	void GetPos(float* px, float* py);
	void SetPos(float _x, float _y);

	void SetHeight(float value);
	void AddHeight(float value);
};
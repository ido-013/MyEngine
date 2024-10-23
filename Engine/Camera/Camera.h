#pragma once

#include <glm/glm.hpp>

#include "../Editor/Editor.h"

class Camera
{
private:
	Camera();
	~Camera();

	Camera(const Camera&) = delete;
	const Camera& operator=(const Camera&) = delete;

	glm::vec2 pos[2];
	float height[2];

	float speed;

	glm::mat3 world_to_ndc_xform;
	Editor::EditorMode mode;

public:
	static Camera& GetInstance()
	{
		static Camera instance;
		return instance;
	}

	void Update();
	void Edit();

	void move();

	void CalculateMatrix();
	glm::mat3& GetMatrix() { return world_to_ndc_xform; }
	
	const glm::vec2& GetPos() const;
	void SetPos(const float& _x, const float& _y);

	void SetHeight(const float& _value);
	void AddHeight(const float& _value);

	const float& GetHeight();
};
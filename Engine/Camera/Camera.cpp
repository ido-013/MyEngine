#include "Camera.h"

#include "../OpenGL/GLHelper.h"

Camera::Camera() : x(0), y(0), height(2), world_to_ndc_xform()
{
	CalculateMatrix();
}

Camera::~Camera()
{

}

void Camera::Update()
{

}

void Camera::CalculateMatrix()
{
	glm::mat3 view_xform(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		-x, -y, 1.f
	);

	GLfloat ar = GLHelper::width / GLHelper::height;

	glm::mat3 camwin_to_ndc_xform(
		1 / (height * GLHelper::width), 0.f, 0.f,
		0.f, 1 / (height * GLHelper::height), 0.f,
		0.f, 0.f, 1.f
	);

	world_to_ndc_xform = camwin_to_ndc_xform * view_xform;
}

void Camera::GetPos(float* px, float* py)
{
	*px = x;
	*py = y;
}

void Camera::SetPos(float _x, float _y)
{
	x = _x;
	y = _y;

	CalculateMatrix();
}

void Camera::AddHeight(float value)
{
	height += value;

	CalculateMatrix();
}

void Camera::SetHeight(float value)
{
	height = value;

	CalculateMatrix();
}
#include "Camera.h"

#include "../OpenGL/GLHelper.h"

Camera::Camera() : pos(), height{ 1, 1 }, speed(300), world_to_ndc_xform()
{
	CalculateMatrix();
}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	mode = Editor::GetInstance().GetMode();

	move();
}

void Camera::Edit()
{
	if (ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
		return;

	ImGui::SetNextWindowSize({ 280, 80 });
	ImGui::Begin("Camera", 0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat2("Position", &pos[mode][0]);
	ImGui::SliderFloat("Height", &height[mode], 1, 8);

	ImGui::End();

	CalculateMatrix();
}

void Camera::move()
{
	float dt = (float)GLHelper::delta_time;

	if (GLHelper::keystateW)
	{
		pos[mode].y += speed * dt;
	}

	if (GLHelper::keystateA)
	{
		pos[mode].x -= speed * dt;
	}

	if (GLHelper::keystateS)
	{
		pos[mode].y -= speed * dt;
	}

	if (GLHelper::keystateD)
	{
		pos[mode].x += speed * dt;
	}

	CalculateMatrix();
}

void Camera::CalculateMatrix()
{
	

	glm::mat3 view_xform(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		-pos[mode].x, -pos[mode].y, 1.f
	);

	glm::mat3 camwin_to_ndc_xform(
		2 / (height[mode] * GLHelper::width), 0.f, 0.f,
		0.f, 2 / (height[mode] * GLHelper::height), 0.f,
		0.f, 0.f, 1.f
	);

	world_to_ndc_xform = camwin_to_ndc_xform * view_xform;
}

const glm::vec2& Camera::GetPos() const
{
	return pos[mode];
}

void Camera::SetPos(const float& _x, const float& _y)
{
	pos[mode].x = _x;
	pos[mode].y = _y;
}

void Camera::AddHeight(const float& _value)
{
	height[mode] += _value;
}

const float& Camera::GetHeight()
{
	return height[mode];
}

void Camera::SetHeight(const float& _value)
{
	height[mode] = _value;
}
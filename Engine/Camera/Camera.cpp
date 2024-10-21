#include "Camera.h"

#include "../OpenGL/GLHelper.h"

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../Editor/Editor.h"

Camera::Camera() : pos(), editPos(), playPos(), height(1), speed(300), world_to_ndc_xform()
{
	CalculateMatrix();
}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	float dt = GLHelper::delta_time;

	if (Editor::GetInstance().GetMode() == Editor::EDIT)
	{
		if (GLHelper::keystateW)
		{
			editPos.y += speed * dt;
		}

		if (GLHelper::keystateA)
		{
			editPos.x -= speed * dt;
		}

		if (GLHelper::keystateS)
		{
			editPos.y -= speed * dt;
		}

		if (GLHelper::keystateD)
		{
			editPos.x += speed * dt;
		}

		pos = editPos;
	}
	else
	{
		if (GLHelper::keystateW)
		{
			playPos.y += speed * dt;
		}

		if (GLHelper::keystateA)
		{
			playPos.x -= speed * dt;
		}

		if (GLHelper::keystateS)
		{
			playPos.y -= speed * dt;
		}

		if (GLHelper::keystateD)
		{
			playPos.x += speed * dt;
		}

		pos = playPos;
	}

	CalculateMatrix();
}

void Camera::Edit()
{
	ImGui::SetNextWindowSize({ 280, 80 });
	ImGui::Begin("Camera", 0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat2("Position", &pos[0]);
	ImGui::SliderFloat("Height", &height, 1, 8);

	ImGui::End();
}

void Camera::CalculateMatrix()
{
	glm::mat3 view_xform(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		-pos.x, -pos.y, 1.f
	);

	glm::mat3 camwin_to_ndc_xform(
		2 / (height * GLHelper::width), 0.f, 0.f,
		0.f, 2 / (height * GLHelper::height), 0.f,
		0.f, 0.f, 1.f
	);

	world_to_ndc_xform = camwin_to_ndc_xform * view_xform;
}

const glm::vec2& Camera::GetPos()
{
	return pos;
}

void Camera::SetPos(const float& _x, const float& _y)
{
	pos.x = _x;
	pos.y = _y;
}

void Camera::AddHeight(const float& _value)
{
	height += _value;
}

const float& Camera::GetHeight()
{
	return height;
}

void Camera::SetHeight(const float& _value)
{
	height = _value;
}
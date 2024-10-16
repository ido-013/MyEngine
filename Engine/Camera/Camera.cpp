#include "Camera.h"

#include "../OpenGL/GLHelper.h"

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../Editor/Editor.h"

Camera::Camera() : pos(), height(2), speed(300), world_to_ndc_xform()
{

}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	if (Editor::GetInstance().GetMode() == Editor::EDIT)
	{
		float dt = GLHelper::delta_time;

		if (ImGui::IsKeyPressed(ImGuiKey_W))
		{
			pos.y += speed * dt;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_A))
		{
			pos.x -= speed * dt;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_S))
		{
			pos.y -= speed * dt;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_D))
		{
			pos.x += speed * dt;
		}
	}

	CalculateMatrix();
}

void Camera::Info()
{
	ImGui::SetNextWindowSize({ 280, 80 });
	ImGui::Begin("Camera", 0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat2("Position", &pos[0]);
	ImGui::SliderFloat("Height", &height, 1, 16);

	ImGui::End();
}

void Camera::CalculateMatrix()
{
	glm::mat3 view_xform(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		-pos.x, -pos.y, 1.f
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
	*px = pos.x;
	*py = pos.y;
}

void Camera::SetPos(float _x, float _y)
{
	pos.x = _x;
	pos.y = _y;
}

void Camera::AddHeight(float value)
{
	height += value;
}

void Camera::SetHeight(float value)
{
	height = value;
}
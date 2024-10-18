#include "Camera.h"

#include "../OpenGL/GLHelper.h"

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../Editor/Editor.h"

Camera::Camera() : pos(), height(2), speed(300), world_to_ndc_xform()
{
	CalculateMatrix();
}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	if (Editor::GetInstance().GetMode() == Editor::EDIT)
	{
		float dt = GLHelper::delta_time;

		if (GLHelper::keystateW)
		{
			pos.y += speed * dt;
		}

		if (GLHelper::keystateA)
		{
			pos.x -= speed * dt;
		}

		if (GLHelper::keystateS)
		{
			pos.y -= speed * dt;
		}

		if (GLHelper::keystateD)
		{
			pos.x += speed * dt;
		}
	}

	CalculateMatrix();
}

void Camera::Window()
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

void Camera::GetPos(float* _px, float* _py)
{
	*_px = pos.x;
	*_py = pos.y;
}

void Camera::SetPos(float _x, float _y)
{
	pos.x = _x;
	pos.y = _y;
}

void Camera::AddHeight(float _value)
{
	height += _value;
}

void Camera::SetHeight(float _value)
{
	height = _value;
}
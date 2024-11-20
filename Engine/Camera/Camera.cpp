#include "Camera.h"

#include "../OpenGL/GLHelper.h"

Camera::Camera() : pos{initPos, initPos}, maxHeight(4), height{1, 1}, onMove{false, false}, speed(300), world_to_ndc_xform()
{
	CalculateMatrix();
}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	mode = Editor::GetInstance().GetMode();

	Move();

	CalculateMatrix();
}

void Camera::Edit()
{
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::InputFloat2("Position", &pos[mode][0]);
		ImGui::SliderFloat("Height", &height[mode], 1, maxHeight);
		ImGui::Checkbox("Move_Edit", &onMove[Editor::EDIT]);
		ImGui::Checkbox("Move_Play", &onMove[Editor::PLAY]);

		CalculateMatrix();

		ImGui::TreePop();
	}
}

void Camera::Move()
{
	if (ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId) || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		return;

	if (!onMove[mode])
		return;
	
	float dt = (float)GLHelper::delta_time;
	float padding = 30;

	if (GLHelper::mouseWindowPos.y < padding)
	{
		pos[mode].y += speed * dt;
	}

	if (GLHelper::mouseWindowPos.x < padding)
	{
		pos[mode].x -= speed * dt;
	}

	if (GLHelper::mouseWindowPos.y > GLHelper::height - padding)
	{
		pos[mode].y -= speed * dt;
	}

	if (GLHelper::mouseWindowPos.x > GLHelper::width - padding)
	{
		pos[mode].x += speed * dt;
	}

	if (GLHelper::keyState[GLFW_KEY_SPACE])
	{
		pos[mode] = initPos;
	}
}

void Camera::CalculateMatrix()
{
	glm::mat4 view_xform(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		-pos[mode].x, -pos[mode].y, 0.f, 1.f
	);

	glm::mat4 camwin_to_ndc_xform(
		2 / (height[mode] * GLHelper::width), 0.f, 0.f, 0.f,
		0.f, 2 / (height[mode] * GLHelper::height), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
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
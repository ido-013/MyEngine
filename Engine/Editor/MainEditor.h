#pragma once

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

void TopBar()
{
	ImGui::BeginMainMenuBar();

	ImGui::Text("Hello World");

	ImGui::EndMainMenuBar();
}
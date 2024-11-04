#include "Util.h"

#include <cstring>
#include <filesystem>

#include "../Imgui/imgui_inc.h"

#include "../OpenGL/GLHelper.h"

bool FileSelectComboOnce(
	std::string& _result,
	const std::string& _label,
	const std::string& _currentFilename,
	const std::string& _filePath,
	const std::string& _filter
)
{
	if (ImGui::BeginCombo(_label.c_str(), _currentFilename.c_str()))
	{
		for (const auto& entry : std::filesystem::directory_iterator(_filePath))
		{
			if (entry.path().stem().string().compare("temp") == 0)
				continue;

			std::string filename = entry.path().filename().string();
			std::string extension = entry.path().extension().string();

			if (!(_filter.empty()) && extension.compare(_filter) != 0)
				continue;

			if (ImGui::MenuItem(filename.c_str()))
			{
				_result = filename;
				ImGui::EndCombo();
				return true;
			}
		}

		ImGui::EndCombo();
	}

	return false;
}

void FileSelectCombo(std::string& _result, const std::string& _label, const std::string& _currentFilename, const std::string& _filePath, const std::string& _filter)
{
	std::string temp;

	if (FileSelectComboOnce(temp, _label, _currentFilename, _filePath, _filter))
	{
		_result = temp;
	}
}

void ClearBuffer(char* _buffer, const int& _size)
{
	memset(_buffer, '\0', _size);
}

bool SameLineButton(const char* _label)
{
	ImGui::SameLine();
	return ImGui::Button(_label);
}

bool ClosePopupButton()
{
	bool ret = ImGui::Button("Close") || GLHelper::keyState[GLFW_KEY_ESCAPE];
	GLHelper::keyState[GLFW_KEY_ESCAPE] = GL_FALSE;
	return ret;
}

bool ClosePopupSameLineButton()
{
	bool ret = SameLineButton("Close") || GLHelper::keyState[GLFW_KEY_ESCAPE];
	GLHelper::keyState[GLFW_KEY_ESCAPE] = GL_FALSE;
	return ret;
}

const char* GetKeyName(int _key)
{
	const char* keyName = glfwGetKeyName(_key, 0);
	
	if (keyName != NULL)
	{
		return keyName;
	}

	else
	{
		switch (_key)
		{
		case GLFW_KEY_UP:
			return "up";

		case GLFW_KEY_DOWN:
			return "down";

		case GLFW_KEY_RIGHT:
			return "right";

		case GLFW_KEY_LEFT:
			return "left";

		default:
			return "##";
		}
	}
}

void KeyChangePopup(const char* _text, int& _key)
{
	ImGui::Text(_text);

	if (SameLineButton((std::string(GetKeyName(_key)) + "##" + _text).c_str()))
	{
		ImGui::OpenPopup(_text);
	}

	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter());

	if (ImGui::BeginPopupModal(_text, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Press the key...");

		if (GLHelper::currentKey != -1)
		{
			_key = GLHelper::currentKey;
			ImGui::CloseCurrentPopup();
		}

		if (ClosePopupButton())
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

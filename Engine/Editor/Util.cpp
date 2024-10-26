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

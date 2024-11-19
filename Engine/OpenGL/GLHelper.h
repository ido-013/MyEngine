#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stack>
#include <map>

struct GLHelper
{
	static bool Init(GLint _width, GLint _height, std::string _title);
	static void Exit();

	static void error_cb(int _error, char const* _description);
	static void fbsize_cb(GLFWwindow* _ptr_win, int _width, int _height);

	static void key_cb(GLFWwindow* _pwin, int _key, int _scancode, int _action, int _mod);
	static void mousebutton_cb(GLFWwindow* _pwin, int _button, int _action, int _mod);
	static void mousescroll_cb(GLFWwindow* _pwin, double _xoffset, double _yoffset);
	static void mousepos_cb(GLFWwindow* _pwin, double _xpos, double _ypos);

	static void setup_event_callbacks();

	static void update_time(double _fpsCalcInt = 1.0);

	static GLint width, height;
	static GLdouble fps;
	static GLdouble delta_time;
	static std::string title;
	static GLFWwindow* ptr_window;

	static std::map<int, GLboolean> keyState;
	static std::stack<int> keyStateStack;
	static std::map<int, GLboolean> ctrlKeyState;
	static int currentKey;

	static glm::vec2 mousePos;
	static glm::vec2 mouseWindowPos;
	static GLboolean mousestateLeft;
};
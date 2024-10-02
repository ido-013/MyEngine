#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

struct GLHelper
{
	static bool Init(GLint width, GLint height, std::string title);
	static void Exit();

	static void error_cb(int error, char const* description);
	static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);

	static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
	static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

	static void setup_event_callbacks();

	static void update_time(double fpsCalcInt = 1.0);

	static GLint width, height;
	static GLdouble fps;
	static GLdouble delta_time;
	static std::string title;
	static GLFWwindow* ptr_window;

	static GLboolean keystateW;
	static GLboolean keystateA;
	static GLboolean keystateS;
	static GLboolean keystateD;
};
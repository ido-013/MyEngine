#pragma once

#include <glad/glad.h>
#include <string>

struct GLModel {
	GLenum primitive_type;
	GLuint draw_cnt;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void init(std::string); // read mesh data from file ...
	void release(); // return buffers back to GPU ...
};
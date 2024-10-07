#include "GLModel.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void GLModel::init(std::string filename)
{
	std::ifstream ifs{ filename, std::ios::in };
	if (!ifs)
	{
		std::cout << "ERROR: Unable to open mesh file: " << filename << "\n";
		exit(EXIT_FAILURE);
	}
	ifs.seekg(0, std::ios::beg);

	std::vector<glm::vec2> pos_vtx;
	std::vector<GLushort> idx_vtx;

	std::string line;

	GLchar prefix;
	GLushort idx;
	GLboolean on_idx = GL_FALSE;

	while (std::getline(ifs, line))
	{
		std::istringstream line_sstm{ line };

		line_sstm >> prefix;

		switch (prefix)
		{
		case 'v':
		{
			glm::vec2 pos;
			line_sstm >> pos.x >> pos.y;
			
			pos_vtx.emplace_back(pos);

			break;
		}
		case 't':
		{
			if (!on_idx)
			{
				primitive_type = GL_TRIANGLES;
				on_idx = GL_TRUE;
			}

			for (GLint i{ 0 }; i < 3; i++)
			{
				line_sstm >> idx;
				idx_vtx.emplace_back(idx);
			}

			break;
		}
		case 'f':
		{
			if (!on_idx)
			{
				primitive_type = GL_TRIANGLE_FAN;
				on_idx = GL_TRUE;

				for (GLint i{ 0 }; i < 3; i++)
				{
					line_sstm >> idx;
					idx_vtx.emplace_back(idx);
				}
			}
			else
			{
				line_sstm >> idx;
				idx_vtx.emplace_back(idx);
			}

			break;
		}
		default:
			break;
		}
	}

	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(glm::vec2) * pos_vtx.size(),
		nullptr, GL_DYNAMIC_STORAGE_BIT);

	GLsizei position_data_offset = 0;
	GLsizei position_attribute_size = sizeof(glm::vec2);
	GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(pos_vtx.size());

	glNamedBufferSubData(VBO, position_data_offset, position_data_size, pos_vtx.data());

	glCreateVertexArrays(1, &VAO);

	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayVertexBuffer(VAO, 3, VBO, position_data_offset, position_attribute_size);
	glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(VAO, 0, 3);

	draw_cnt = static_cast<GLuint>(idx_vtx.size());

	glCreateBuffers(1, &EBO);
	glNamedBufferStorage(EBO, sizeof(GLushort) * draw_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(VAO, EBO);

	glBindVertexArray(0);
}

void GLModel::release()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

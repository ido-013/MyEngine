#include "GLModel.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
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

	std::vector<GLfloat> pos_vtx;
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
			
			pos_vtx.push_back(pos.x);
			pos_vtx.push_back(pos.y);

			pos_vtx.push_back(pos.x / 2 + 0.5f);
			pos_vtx.push_back(pos.y / 2 + 0.5f);

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

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glNamedBufferStorage(VBO, sizeof(GLfloat) * pos_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(VBO, 0, sizeof(GLfloat) * pos_vtx.size(), pos_vtx.data());

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

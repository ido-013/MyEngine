#include "Grid.h"

#include <glm/gtc/type_ptr.hpp>

#include "../OpenGL/GLHelper.h"

#include "../ResourceManager/ResourceManager.h"
#include "../Camera/Camera.h"
#include "../Editor/Editor.h"

Grid::Grid() : size(50), pos(), viewGrid(true), gridLineColor{ 0.f, 0.f, 0.f, 1.f }
{
	shaderProgram = ResourceManager::GetInstance().GetResourcePointer<GLuint>("base.shd");
    horizontalMesh = ResourceManager::GetInstance().GetResourcePointer<GLModel>("horizontal_line.msh");
    verticalMesh = ResourceManager::GetInstance().GetResourcePointer<GLModel>("vertical_line.msh");
	texobj = ResourceManager::GetInstance().GetResourcePointer<GLuint>("base.png");
}

Grid::~Grid()
{
	ResourceManager::GetInstance().UnloadResource("base.shd");
	ResourceManager::GetInstance().UnloadResource("horizontal_line.msh");
	ResourceManager::GetInstance().UnloadResource("vertical_line.msh");
	ResourceManager::GetInstance().UnloadResource("base.png");
}

const glm::vec2 Grid::GetGridInd(const glm::vec2& _vec)
{
    glm::vec2 res;

    res.x = ((int)_vec.x / size) + (_vec.x < 0 ? -1 : 0);
    res.y = ((int)_vec.y / size) + (_vec.y < 0 ? -1 : 0);

    return res;
}

const glm::vec2 Grid::GetGridPos(const glm::vec2& _vec)
{
    glm::vec2 res = GetGridInd(_vec);

    res.x = res.x * size + (size / 2.f);
    res.y = res.y * size + (size / 2.f);

    return res;
}

const glm::vec2 Grid::GetGridPos(const int& _x, const int& _y)
{
    glm::vec2 res;

    res.x = _x * size + (size / 2.f);
    res.y = _y * size + (size / 2.f);

    return res;
}

void Grid::Draw()
{
    if (!viewGrid)
        return;

    glm::vec2 cameraPos = Camera::GetInstance().GetPos();
    float cameraMaxHeight = Camera::GetInstance().GetMaxHeight();

    float width = GLHelper::width * cameraMaxHeight;
    float height = GLHelper::height * cameraMaxHeight;

    pos.x = ((int)cameraPos.x / size) * size + (cameraPos.x < 0 ? -size : 0);
    pos.y = ((int)cameraPos.y / size) * size + (cameraPos.y < 0 ? -size : 0);

    glLineWidth(1);

    glUseProgram(*shaderProgram);

    GLint loc = glGetUniformLocation(*shaderProgram, "uTex2d");
    if (loc >= 0)
    {
        glUniform1i(loc, 6);
    }

    loc = glGetUniformLocation(*shaderProgram, "uColor");
    if (loc >= 0)
    {
        glUniform4fv(loc, 1, gridLineColor);
    }

    loc = glGetUniformLocation(*shaderProgram, "uCoordZ");
    if (loc >= 0)
    {
        glUniform1f(loc, 1);
    }

    glBindTextureUnit(6, *texobj);
    glBindTexture(GL_TEXTURE_2D, *texobj);
    glBindTexture(GL_TEXTURE_2D, 0);

    loc = glGetUniformLocation(*shaderProgram, "uModel_to_NDC");
    if (loc >= 0)
    {
        // horizontal line
        glBindVertexArray(horizontalMesh->VAO);

        glm::mat4 transformMatrix(
            width, 0.f, 0.f, 0.f,
            0.f, height, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        );

        transformMatrix[3][0] = pos.x;

        for (int i = -size; i < height + size; i += size)
        {
            transformMatrix[3][1] = (float)(i - (height / 2)) + pos.y;
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance().GetMatrix() * transformMatrix));
            glDrawArrays(GL_LINES, 0, 2);
        }

        // vertical line
        transformMatrix[3][1] = pos.y;

        glBindVertexArray(verticalMesh->VAO);

        for (int i = -size; i < width + size; i += size)
        {
            transformMatrix[3][0] = (float)(i - (width / 2)) + pos.x;
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance().GetMatrix() * transformMatrix));
            glDrawArrays(GL_LINES, 0, 2);
        }
    }

    glUseProgram(0);
}

void Grid::Edit()
{
    ImGui::ColorEdit4("Color", gridLineColor);
    ImGui::Checkbox("View Grid Line", &viewGrid);
}

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../OpenGL/GLModel.h"

class Grid
{
private:
    Grid();

    Grid(const Grid&) = delete;
    const Grid& operator=(const Grid&) = delete;

    ~Grid();

    GLuint* shaderProgram;
    GLModel* horizontalMesh;
    GLModel* verticalMesh;
    GLuint* texobj;

    int size;
    glm::vec2 pos;

    bool viewGrid;
    float gridLineColor[4];

    

public:
    static Grid& GetInstance()
    {
        static Grid instance;
        return instance;
    }

    const int& GetSize() { return size; }
    const glm::vec2 GetGridInd(const glm::vec2& _vec);
    const glm::vec2 GetGridPos(const glm::vec2& _vec);
    const glm::vec2 GetGridPos(const int& _x, const int& _y);

    void Draw();
    void Edit();
};

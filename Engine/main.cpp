#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Editor/Editor.h"
#include "OpenGL/GLHelper.h"
#include "GameStateManager/GameStateManager.h"
#include "Level/TestLevel.h"

#include <iostream>

int main()
{
    GLHelper::Init(1600, 900, "My Game!!!");
    Editor::Init();

    GameStateManager& gsm = GameStateManager::GetInstance();
    gsm.ChangeLevel(new level::TestLevel);

    while (!glfwWindowShouldClose(GLHelper::ptr_window))
    {
        GLHelper::update_time();
        gsm.Update();

        glfwPollEvents();

        Editor::Update();

        glfwSwapBuffers(GLHelper::ptr_window);
    }

    gsm.Exit();
    Editor::Exit();
    GLHelper::Exit();

    return 0;
}
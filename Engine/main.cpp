#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGL/GLHelper.h"
#include "GameStateManager/GameStateManager.h"
#include "Level/TestLevel.h"

int main()
{
    GLHelper::Init(800, 600, "My Game!!!");

    GSM::GameStateManager& gsm = GSM::GameStateManager::GetInstance();
    gsm.ChangeLevel(new level::TestLevel);

    while (!glfwWindowShouldClose(GLHelper::ptr_window))
    {
        gsm.Update();
    }

    gsm.Exit();
    GLHelper::Exit();

    return 0;
}
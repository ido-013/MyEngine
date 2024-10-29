#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <crtdbg.h>

#include "Editor/Editor.h"
#include "OpenGL/GLHelper.h"
#include "GameStateManager/GameStateManager.h"
#include "Level/TestLevel.h"

#include "Profiler/Profiler.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GLHelper::Init(1600, 900, "My Game!!!");
    Editor::GetInstance().Init();

    GameStateManager& gsm = GameStateManager::GetInstance();
    gsm.ChangeLevel(new level::TestLevel);

    while (!glfwWindowShouldClose(GLHelper::ptr_window))
    {
        DEBUG_PROFILER_START(__FUNCTION_NAME__);

        GLHelper::update_time();
        gsm.Update();

        glfwPollEvents();

        Editor::GetInstance().Update();

        glfwSwapBuffers(GLHelper::ptr_window);

        DEBUG_PROFILER_END;
    }

    gsm.Exit();
    Editor::GetInstance().Exit();
    GLHelper::Exit();

    return 0;
}
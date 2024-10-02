#include "GameStateManager.h"

#include <glad/glad.h>
#include "../OpenGL/GLHelper.h"

#include "../ComponentManager/ComponentManager.h"
#include "../LogicComponent/LogicComponent.h"
#include "../EngineComponent/EngineComponent.h"
#include "../GraphicComponent/GraphicComponent.h"

#include "../Level/BaseLevel.h"

GSM::GameStateManager::GameStateManager() : previousLevel(nullptr), currentLevel(nullptr) {}

GSM::GameStateManager::~GameStateManager()
{
    if (previousLevel)
        delete previousLevel;

    if (currentLevel)
        delete currentLevel;
}

void GSM::GameStateManager::Init()
{
    if (currentLevel)
    {
        currentLevel->Init();
    }
}

void GSM::GameStateManager::Update()
{
    if (currentLevel)
    {
        ComponentManager<LogicComponent>::GetInstance().Update();
        ComponentManager<EngineComponent>::GetInstance().Update();

        if (currentLevel)
            currentLevel->Update();

        glClear(GL_COLOR_BUFFER_BIT);
        ComponentManager<GraphicComponent>::GetInstance().Update();
        glfwSwapBuffers(GLHelper::ptr_window);
        glfwPollEvents();
    }
}

void GSM::GameStateManager::Exit()
{
    if (currentLevel)
    {
        currentLevel->Exit();
    }
}

void GSM::GameStateManager::ChangeLevel(BaseLevel* newLvl)
{
    delete previousLevel;
    previousLevel = currentLevel;

    //Exit the current level
    Exit();

    //Current level is now the "next" level
    currentLevel = newLvl;

    //initialize the level
    Init();
}

bool GSM::GameStateManager::ShouldExit()
{
    return currentLevel == nullptr;
}
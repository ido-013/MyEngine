#include "GameStateManager.h"

#include <glad/glad.h>
#include "../OpenGL/GLHelper.h"

#include "../ComponentManager/ComponentManager.h"
#include "../LogicComponent/LogicComponent.h"
#include "../EngineComponent/EngineComponent.h"
#include "../GraphicComponent/GraphicComponent.h"

#include "../Level/BaseLevel.h"

GameStateManager::GameStateManager() : previousLevel(nullptr), currentLevel(nullptr) {}

GameStateManager::~GameStateManager()
{
    if (previousLevel)
        delete previousLevel;

    if (currentLevel)
        delete currentLevel;
}

void GameStateManager::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (currentLevel)
    {
        currentLevel->Init();
    }
}

void GameStateManager::Update()
{
    if (currentLevel)
    {
        ComponentManager<LogicComponent>::GetInstance().Update();
        ComponentManager<EngineComponent>::GetInstance().Update();

        if (currentLevel)
            currentLevel->Update();

        glClear(GL_COLOR_BUFFER_BIT);
        ComponentManager<GraphicComponent>::GetInstance().Update();
    }
}

void GameStateManager::Exit()
{
    if (currentLevel)
    {
        currentLevel->Exit();
    }
}

void GameStateManager::ChangeLevel(BaseLevel* newLvl)
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

bool GameStateManager::ShouldExit()
{
    return currentLevel == nullptr;
}
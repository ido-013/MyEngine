#include "GameStateManager.h"

#include <glad/glad.h>
#include "../OpenGL/GLHelper.h"

#include "../ComponentManager/ComponentManager.h"
#include "../CollisionManager/CollisionManager.h"
#include "../EventManager/EventManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Editor/Editor.h"
#include "../Camera/Camera.h"

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
    Camera::GetInstance();

    if (currentLevel)
    {
        currentLevel->Init();
    }
}

void GameStateManager::Update()
{
    if (currentLevel)
    {
        Editor::EditorMode editorMode = Editor::GetInstance().GetMode();

        if (editorMode == Editor::PLAY)
        {
            ComponentManager<LogicComponent>::GetInstance().Update();
            ComponentManager<EngineComponent>::GetInstance().Update();
            CollisionManager::GetInstance().Update();
            EventManager::GetInstance().DispatchAllEvents();
            Camera::GetInstance().Update();
        }


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

    ResourceManager::GetInstance().UnloadAllResource();
    EventManager::GetInstance().DeleteUndispahchEvent();
}

void GameStateManager::ChangeLevel(BaseLevel* _newLvl)
{
    delete previousLevel;
    previousLevel = currentLevel;

    //Exit the current level
    Exit();

    //Current level is now the "next" level
    currentLevel = _newLvl;

    //initialize the level
    Init();
}

bool GameStateManager::ShouldExit()
{
    return currentLevel == nullptr;
}
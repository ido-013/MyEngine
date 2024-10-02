#include "TestLevel.h"

#include <iostream>

#include "../GameStateManager/GameStateManager.h"
#include "../ComponentManager/ComponentManager.h"
#include "../GameObject/GameObject.h"

#include "../GraphicComponent/SpriteComp.h"
#include "../EngineComponent/TransformComp.h"

GameObject* tmp = nullptr;

void level::TestLevel::Init()
{
    tmp = new GameObject;
    TransformComp* t = tmp->AddComponent<TransformComp>();
    t->SetScale({ 0.1f, 0.1f });
    tmp->AddComponent<SpriteComp>();
}

void level::TestLevel::Update()
{
    
}

void level::TestLevel::Exit()
{

}

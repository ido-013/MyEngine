#include "TestLevel.h"

#include <iostream>

#include "../GameStateManager/GameStateManager.h"
#include "../ComponentManager/ComponentManager.h"
#include "../GameObjectManager/GameObjectManager.h"
#include "../Serializer/Serializer.h"

#include "../GraphicComponent/SpriteComp.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../LogicComponent/PlayerComp.h"

#include "../Camera/Camera.h"

GameObject* tmp = nullptr;
GameObject* tmp2 = nullptr;
TransformComp* t = nullptr;
SpriteComp* s = nullptr;

void level::TestLevel::Init()
{
    tmp = GameObjectManager::GetInstance().CreateObject("temp");

    t = tmp->AddComponent<TransformComp>();
    t->SetScale({ 600, 600 });

    s = tmp->AddComponent<SpriteComp>();

    RigidbodyComp* r = tmp->AddComponent<RigidbodyComp>();
    PlayerComp* p = tmp->AddComponent<PlayerComp>();
}

void level::TestLevel::Update()
{    

}

void level::TestLevel::Exit()
{

}

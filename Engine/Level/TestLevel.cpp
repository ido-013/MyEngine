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
TransformComp* t = nullptr;
SpriteComp* s = nullptr;

void level::TestLevel::Init()
{
    Camera::GetInstance().SetPos(0, 0);

    tmp = GameObjectManager::GetInstance().AddObject("temp");

    t = tmp->AddComponent<TransformComp>();
    t->SetScale({ 600, 600 });

    s = tmp->AddComponent<SpriteComp>();
    s->SetTexture("GSwftl7boAADleI.jfif");
    s->SetColor(255, 255, 255);
    s->SetAlpha(1.f);

    RigidbodyComp* r = tmp->AddComponent<RigidbodyComp>();
    PlayerComp* p = tmp->AddComponent<PlayerComp>();

    Serializer::GetInstance().SaveLevel("t");
}

void level::TestLevel::Update()
{    

}

void level::TestLevel::Exit()
{

}

#include "TestLevel.h"

#include <iostream>

#include "../GameStateManager/GameStateManager.h"
#include "../ComponentManager/ComponentManager.h"
#include "../GameObject/GameObject.h"

#include "../GraphicComponent/SpriteComp.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../LogicComponent/PlayerComp.h"

#include "../Camera/Camera.h"

GameObject* tmp = nullptr;
GameObject* tmp2 = nullptr;

void level::TestLevel::Init()
{
    Camera::GetInstance().SetPos(0, 0);

    tmp = new GameObject;
    TransformComp* t = tmp->AddComponent<TransformComp>();
    t->SetScale({ 600, 600 });

    SpriteComp* s = tmp->AddComponent<SpriteComp>();
    s->SetTexture("../Assets/GSwftl7boAADleI.jfif");
    s->SetColor(255, 255, 255);
    s->SetAlpha(1.f);

    RigidbodyComp* r = tmp->AddComponent<RigidbodyComp>();
    PlayerComp* p = tmp->AddComponent<PlayerComp>();
}

void level::TestLevel::Update()
{    

}

void level::TestLevel::Exit()
{

}

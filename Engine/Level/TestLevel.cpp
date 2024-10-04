#include "TestLevel.h"

#include <iostream>

#include "../GameStateManager/GameStateManager.h"
#include "../ComponentManager/ComponentManager.h"
#include "../GameObject/GameObject.h"

#include "../GraphicComponent/SpriteComp.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../LogicComponent/PlayerComp.h"

GameObject* tmp = nullptr;
GameObject* tmp2 = nullptr;

void level::TestLevel::Init()
{
    tmp = new GameObject;
    TransformComp* t = tmp->AddComponent<TransformComp>();
    t->SetScale({ 0.7f, 0.5f });

    SpriteComp* s = tmp->AddComponent<SpriteComp>();

    RigidbodyComp* r = tmp->AddComponent<RigidbodyComp>();
    
    PlayerComp* p = tmp->AddComponent<PlayerComp>();

    tmp2 = new GameObject;
    t = tmp2->AddComponent<TransformComp>();
    t->SetScale({ 0.7f, 0.5f });
    t->SetRot(90);

    s = tmp2->AddComponent<SpriteComp>();
}

void level::TestLevel::Update()
{
    //tmp2->GetComponent<TransformComp>()->SetRot(tmp2->GetComponent<TransformComp>()->GetRot() + 1);
}

void level::TestLevel::Exit()
{

}

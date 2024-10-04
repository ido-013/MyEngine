#include "Registry.h"
#include "../Components.h"

Registry::Registry()
{
    // Register ALL the functions
    rttiMap.insert({ TransformComp::TypeName, &TransformComp::CreateTransformComponent });
    rttiMap.insert({ PlayerComp::TypeName, &PlayerComp::CreatePlayerComponent });
    rttiMap.insert({ SpriteComp::TypeName, &SpriteComp::CreateSpriteComponent });
    rttiMap.insert({ RigidbodyComp::TypeName, &RigidbodyComp::CreateRigidBodyComponent });
}

BaseRTTI* Registry::FindAndCreate(const std::string& type, GameObject* owner)
{
    BaseRTTI* ptr = nullptr;
    if (rttiMap.find(type) != rttiMap.end())
        ptr = rttiMap[type](owner);
    else
        throw std::invalid_argument("Registry FindAndCreate Invalid typeName " + type);

    return ptr;
}

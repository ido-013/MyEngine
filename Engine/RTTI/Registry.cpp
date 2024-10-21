#include "Registry.h"
#include "../Components.h"

Registry::Registry()
{
    // Register ALL the functions
    rttiMap.insert({ TransformComp::TypeName, &TransformComp::CreateTransformComponent });
    rttiMap.insert({ PlayerComp::TypeName, &PlayerComp::CreatePlayerComponent });
    rttiMap.insert({ SpriteComp::TypeName, &SpriteComp::CreateSpriteComponent });
    rttiMap.insert({ RigidbodyComp::TypeName, &RigidbodyComp::CreateRigidBodyComponent });
    rttiMap.insert({ ColliderComp::TypeName, &ColliderComp::CreateColliderComponent });
}

BaseRTTI* Registry::FindAndCreate(const std::string& _type, GameObject* _owner)
{
    BaseRTTI* ptr = nullptr;
    if (rttiMap.find(_type) != rttiMap.end())
        ptr = rttiMap[_type](_owner);
    else
        throw std::invalid_argument("Registry FindAndCreate Invalid typeName " + _type);

    return ptr;
}

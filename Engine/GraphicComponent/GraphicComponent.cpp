#include "GraphicComponent.h"
#include "../ComponentManager/ComponentManager.h"

GraphicComponent::GraphicComponent(GameObject* _owner) : BaseComponent(_owner)
{
	ComponentManager<GraphicComponent>::GetInstance().AddComp(this);
}

GraphicComponent::~GraphicComponent()
{
	ComponentManager<GraphicComponent>::GetInstance().DelComp(this);
}

#include "GameObject.h"
#include "../BaseComponent.h"

#include "../GraphicComponent/SpriteComp.h"

GameObject::GameObject(std::string _name) : name(_name), prefabName(), layerName("Default"), selected(false)
{

}

GameObject::~GameObject()
{
	for (auto& it : component)
	{
		if (it.second)
			delete it.second;
	}

	component.clear();
}

bool GameObject::IsHaveComponent(const std::string& _typeName)
{
	return component.find(_typeName) != component.end();
}

void GameObject::SetLayerName(const std::string& _name)
{
	layerName = _name;
	
	SpriteComp* s = GetComponent<SpriteComp>();
	if (s)
	{
		s->SetDepth();
	}
}

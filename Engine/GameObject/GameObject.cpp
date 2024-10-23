#include "GameObject.h"
#include "../BaseComponent.h"

GameObject::GameObject(std::string _name) : name(_name), prefabName(), selected(false)
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

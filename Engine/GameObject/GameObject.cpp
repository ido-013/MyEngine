#include "GameObject.h"
#include "../BaseComponent.h"

GameObject::GameObject(std::string _name) : name(_name), prefabName()
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

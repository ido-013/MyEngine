#include "GameObject.h"
#include "../BaseComponent.h"

GameObject::GameObject()
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

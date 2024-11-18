#include "GameObject.h"

#include "../Components.h"

GameObject::GameObject(std::string _name) : name(_name), prefabName(), selected(false)
{
	SetLayer(LayerManager::maxLayerInd);
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

void GameObject::SetLayer(const int& _layer)
{
	layer = _layer;
	layerName = LayerManager::GetInstance().GetName(_layer);

	SpriteComp* s = GetComponent<SpriteComp>();
	if (s)
	{
		s->SetDepth();
	}
}